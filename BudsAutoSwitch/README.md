# BudsAutoSwitch

이 기기에서 미디어 재생이 시작되면, 등록해 둔 Galaxy Buds4 Pro(또는 다른 블루투스 헤드셋)를
A2DP로 자동 연결하여 다른 기기에서 듣고 있던 소리를 이 기기로 전환합니다.

이전 버전(.NET MAUI 기반)을 네이티브 Kotlin/Android 프로젝트로 새로 작성했습니다.

## 동작 방식

1. **재생 감지**: `MediaPlaybackListenerService`는 `NotificationListenerService`를 상속합니다.
   `MediaSessionManager`에서 활성 미디어 세션 목록을 가져오려면 알림 접근 권한이 필요하기
   때문입니다. 모든 앱의 미디어 세션을 감시하다가 `STATE_PLAYING`으로 바뀌는 시점을 감지합니다.
2. **자동 연결**: 재생이 시작되면 `BudsConnector`가 등록된 기기의 A2DP 연결 상태를 확인하고,
   연결되어 있지 않으면 연결을 시도합니다. Android 공개 API에는 "이미 페어링된 기기에 프로필
   연결을 거는" 기능이 없어서, 숨겨진 `BluetoothA2dp#connect(BluetoothDevice)` 메서드를
   리플렉션으로 호출합니다.
3. **로그**: 모든 시도/실패 사유가 `EventLog`에 기록되고 메인 화면 하단에 표시됩니다.
   기존 앱이 "되는지 안 되는지 모르겠다"던 문제를 해결하기 위해, 권한 부족이나
   `SecurityException` 등 실패 원인을 화면에서 바로 볼 수 있습니다.

## 알아둘 점 (한계)

- `BluetoothA2dp#connect()`는 비공개(hidden) API입니다. `BLUETOOTH_CONNECT` 권한만으로
  대부분의 기기/Android 버전에서 동작하지만, 일부 OEM 펌웨어(특히 Android 12+ 에서
  `BLUETOOTH_PRIVILEGED`를 엄격히 검사하는 경우)는 `SecurityException`을 던지며 거부할 수
  있습니다. 이 경우 앱 화면 로그에 실패 사유가 그대로 출력되니, 만약 항상 실패한다면 루팅
  없이는 자동 연결이 불가능한 기기라는 뜻입니다.
- 연결 "요청"을 보내는 시점만 확인하며, 실제 연결 성공 여부(ACL 연결 완료)는 별도로
  추적하지 않습니다.
- 같은 재생 시작 이벤트가 짧은 시간 내 반복 발생하는 것을 막기 위해 10초 쿨다운을 둡니다.

## 사용 방법

1. 앱 실행 후 **블루투스 권한 요청** → **알림 접근 권한 설정**을 순서대로 허용합니다.
2. **기기 선택**에서 페어링된 Buds4 Pro를 선택합니다.
3. **감시 시작**을 누르면 이 기기에서 재생이 시작될 때마다 자동 연결을 시도합니다.

## 빌드

Android Studio에서 `BudsAutoSwitch/` 폴더를 프로젝트로 열면 Gradle Wrapper가 자동으로
생성됩니다. CLI로 빌드하려면 Android SDK(`ANDROID_HOME`)가 설정된 환경에서:

```sh
./gradlew assembleDebug
```

> 이 저장소를 작성한 샌드박스 환경은 `dl.google.com` / `maven.google.com`에 대한 네트워크
> 접근이 차단되어 있어 이번 변경 사항은 빌드/실행 테스트를 직접 수행하지 못했습니다.
> Android Studio 등 SDK가 갖춰진 환경에서 빌드 확인이 필요합니다.
