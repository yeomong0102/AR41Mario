package dev.zzmar.budsautoswitch

import android.content.ComponentName
import android.content.Intent
import android.media.session.MediaController
import android.media.session.MediaSession
import android.media.session.MediaSessionManager
import android.media.session.PlaybackState
import android.os.Build
import android.service.notification.NotificationListenerService
import android.service.notification.StatusBarNotification
import dev.zzmar.budsautoswitch.Prefs.isServiceEnabled
import dev.zzmar.budsautoswitch.Prefs.targetDeviceAddress

/**
 * Notification-listener service that doubles as our "is media playing on
 * this device" detector, since [MediaSessionManager.getActiveSessions]
 * requires the calling component to hold notification listener access.
 *
 * When any app's media session transitions to STATE_PLAYING, we try to
 * connect the saved Buds4 Pro device over A2DP so playback switches to it.
 */
class MediaPlaybackListenerService : NotificationListenerService() {

    private lateinit var mediaSessionManager: MediaSessionManager
    private val controllerCallbacks = mutableMapOf<MediaSession.Token, MediaController.Callback>()
    private var lastAttemptAt = 0L

    private val sessionsChangedListener =
        MediaSessionManager.OnActiveSessionsChangedListener { controllers ->
            registerControllers(controllers ?: emptyList())
        }

    override fun onListenerConnected() {
        super.onListenerConnected()
        EventLog.add("알림 접근 권한 연결됨 - 재생 감지 시작")
        startForegroundServiceCompat()

        mediaSessionManager = getSystemService(MediaSessionManager::class.java)
        val componentName = ComponentName(this, MediaPlaybackListenerService::class.java)
        mediaSessionManager.addOnActiveSessionsChangedListener(sessionsChangedListener, componentName)
        registerControllers(mediaSessionManager.getActiveSessions(componentName))
    }

    override fun onListenerDisconnected() {
        super.onListenerDisconnected()
        EventLog.add("알림 접근 권한 연결 해제됨")
        try {
            mediaSessionManager.removeOnActiveSessionsChangedListener(sessionsChangedListener)
        } catch (e: Exception) {
            // already disconnected
        }
        controllerCallbacks.clear()
        stopService(Intent(this, BudsForegroundService::class.java))
    }

    private fun startForegroundServiceCompat() {
        val intent = Intent(this, BudsForegroundService::class.java)
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            startForegroundService(intent)
        } else {
            startService(intent)
        }
    }

    private fun registerControllers(controllers: List<MediaController>) {
        val currentTokens = controllers.map { it.sessionToken }.toSet()
        controllerCallbacks.keys.retainAll(currentTokens)

        for (controller in controllers) {
            if (controllerCallbacks.containsKey(controller.sessionToken)) continue

            val callback = object : MediaController.Callback() {
                override fun onPlaybackStateChanged(state: PlaybackState?) {
                    if (state?.state == PlaybackState.STATE_PLAYING) {
                        onPlaybackStarted(controller.packageName)
                    }
                }
            }
            controller.registerCallback(callback)
            controllerCallbacks[controller.sessionToken] = callback

            if (controller.playbackState?.state == PlaybackState.STATE_PLAYING) {
                onPlaybackStarted(controller.packageName)
            }
        }
    }

    private fun onPlaybackStarted(packageName: String) {
        if (!applicationContext.isServiceEnabled) return

        val now = System.currentTimeMillis()
        if (now - lastAttemptAt < COOLDOWN_MS) return
        lastAttemptAt = now

        val address = applicationContext.targetDeviceAddress
        if (address == null) {
            EventLog.add("[$packageName] 재생 감지됨, 대상 기기가 설정되지 않음")
            return
        }

        val device = BudsConnector.getBondedDevice(applicationContext, address)
        if (device == null) {
            EventLog.add("[$packageName] 재생 감지됨, 등록된 기기를 찾을 수 없음 ($address)")
            return
        }

        EventLog.add("[$packageName] 재생 감지됨 -> ${BudsConnector.displayName(device)} 연결 시도")
        BudsConnector.connect(applicationContext, device) { _, message ->
            EventLog.add(message)
        }
    }

    override fun onNotificationPosted(sbn: StatusBarNotification?) {}
    override fun onNotificationRemoved(sbn: StatusBarNotification?) {}

    companion object {
        private const val COOLDOWN_MS = 10_000L
    }
}
