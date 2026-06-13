package dev.zzmar.budsautoswitch

import android.bluetooth.BluetoothA2dp
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothManager
import android.bluetooth.BluetoothProfile
import android.content.Context
import java.lang.reflect.InvocationTargetException

/**
 * Helper around the Bluetooth A2DP profile.
 *
 * There is no public Android API that lets a third-party app initiate a
 * profile connection to an already-bonded device - the system only exposes
 * [BluetoothA2dp.getConnectedDevices]. To actually trigger the switch we call
 * the hidden BluetoothA2dp#connect(BluetoothDevice) method via reflection.
 * This works on many devices when the app holds BLUETOOTH_CONNECT, but some
 * OEM builds restrict it to system apps and throw SecurityException - in
 * that case [connect] reports the failure via its callback so it is visible
 * in the on-screen log instead of silently doing nothing.
 */
object BudsConnector {

    fun getAdapter(context: Context): BluetoothAdapter? {
        val manager = context.getSystemService(Context.BLUETOOTH_SERVICE) as? BluetoothManager
        return manager?.adapter
    }

    fun getBondedDevices(context: Context): List<BluetoothDevice> {
        return try {
            getAdapter(context)?.bondedDevices?.toList() ?: emptyList()
        } catch (e: SecurityException) {
            emptyList()
        }
    }

    fun getBondedDevice(context: Context, address: String): BluetoothDevice? {
        return getBondedDevices(context).firstOrNull { it.address == address }
    }

    /**
     * Attempts to connect the A2DP profile to [device]. Returns the result
     * via [callback] (called on the binder thread) once the attempt has been
     * made - it does not wait for the connection to actually complete.
     */
    fun connect(context: Context, device: BluetoothDevice, callback: (Boolean, String) -> Unit) {
        val adapter = getAdapter(context)
        if (adapter == null) {
            callback(false, "블루투스 어댑터를 사용할 수 없습니다")
            return
        }

        val listener = object : BluetoothProfile.ServiceListener {
            override fun onServiceConnected(profile: Int, proxy: BluetoothProfile) {
                try {
                    val a2dp = proxy as BluetoothA2dp
                    val alreadyConnected = a2dp.connectedDevices.any { it.address == device.address }
                    if (alreadyConnected) {
                        callback(true, "이미 연결되어 있음: ${displayName(device)}")
                        return
                    }

                    val method = BluetoothA2dp::class.java.getMethod(
                        "connect", BluetoothDevice::class.java
                    )
                    val result = method.invoke(a2dp, device) as Boolean
                    if (result) {
                        callback(true, "연결 요청 전송됨: ${displayName(device)}")
                    } else {
                        callback(false, "연결 요청이 거부됨 (connect()가 false 반환): ${displayName(device)}")
                    }
                } catch (e: InvocationTargetException) {
                    val cause = e.cause ?: e
                    callback(false, "연결 요청 실패: ${cause.javaClass.simpleName}: ${cause.message}")
                } catch (e: Exception) {
                    callback(false, "연결 요청 실패: ${e.javaClass.simpleName}: ${e.message}")
                } finally {
                    adapter.closeProfileProxy(profile, proxy)
                }
            }

            override fun onServiceDisconnected(profile: Int) {}
        }

        try {
            val started = adapter.getProfileProxy(context, listener, BluetoothProfile.A2DP)
            if (!started) {
                callback(false, "A2DP 프로필 프록시를 가져올 수 없습니다")
            }
        } catch (e: SecurityException) {
            callback(false, "블루투스 권한이 없습니다: ${e.message}")
        }
    }

    fun displayName(device: BluetoothDevice): String {
        return try {
            device.name ?: device.address
        } catch (e: SecurityException) {
            device.address
        }
    }
}
