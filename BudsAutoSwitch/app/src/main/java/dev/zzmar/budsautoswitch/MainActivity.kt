package dev.zzmar.budsautoswitch

import android.Manifest
import android.content.Intent
import android.content.pm.PackageManager
import android.os.Build
import android.os.Bundle
import android.provider.Settings
import androidx.activity.result.contract.ActivityResultContracts
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.NotificationManagerCompat
import androidx.core.content.ContextCompat
import dev.zzmar.budsautoswitch.Prefs.isServiceEnabled
import dev.zzmar.budsautoswitch.Prefs.targetDeviceAddress
import dev.zzmar.budsautoswitch.Prefs.targetDeviceName
import dev.zzmar.budsautoswitch.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    private val logListener: (String) -> Unit = { text ->
        binding.textLog.text = text
    }

    private val requestBluetoothPermission =
        registerForActivityResult(ActivityResultContracts.RequestPermission()) {
            refreshStatus()
        }

    private val requestNotificationPermission =
        registerForActivityResult(ActivityResultContracts.RequestPermission()) {
            refreshStatus()
        }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        binding.btnSelectDevice.setOnClickListener { showDevicePicker() }
        binding.btnGrantBluetoothPermission.setOnClickListener { requestBluetoothPermissions() }
        binding.btnGrantNotificationAccess.setOnClickListener {
            startActivity(Intent(Settings.ACTION_NOTIFICATION_LISTENER_SETTINGS))
        }
        binding.btnToggleService.setOnClickListener { toggleService() }
    }

    override fun onResume() {
        super.onResume()
        EventLog.addListener(logListener)
        logListener(EventLog.currentText())
        refreshStatus()
    }

    override fun onPause() {
        super.onPause()
        EventLog.removeListener(logListener)
    }

    private fun refreshStatus() {
        val address = targetDeviceAddress
        val name = targetDeviceName
        binding.textSelectedDevice.text = if (address != null) {
            "$name ($address)"
        } else {
            getString(R.string.no_device_selected)
        }

        val hasBluetoothPermission = hasBluetoothConnectPermission()
        binding.textBluetoothPermission.text = if (hasBluetoothPermission) {
            getString(R.string.bluetooth_permission_granted)
        } else {
            getString(R.string.bluetooth_permission_required)
        }
        binding.btnGrantBluetoothPermission.isEnabled = !hasBluetoothPermission

        val hasNotificationAccess = isNotificationListenerEnabled()
        binding.textNotificationAccess.text = if (hasNotificationAccess) {
            getString(R.string.notification_access_granted)
        } else {
            getString(R.string.notification_access_required)
        }

        val enabled = isServiceEnabled
        binding.textServiceStatus.text = if (enabled) {
            getString(R.string.service_running)
        } else {
            getString(R.string.service_stopped)
        }
        binding.btnToggleService.text = if (enabled) {
            getString(R.string.btn_stop_service)
        } else {
            getString(R.string.btn_start_service)
        }
    }

    private fun toggleService() {
        if (!isServiceEnabled) {
            if (targetDeviceAddress == null) {
                EventLog.add("대상 기기를 먼저 선택하세요")
                refreshStatus()
                return
            }
            if (!hasBluetoothConnectPermission()) {
                EventLog.add("블루투스 권한을 먼저 허용하세요")
                requestBluetoothPermissions()
                return
            }
            if (!isNotificationListenerEnabled()) {
                EventLog.add("알림 접근 권한을 먼저 허용하세요")
                startActivity(Intent(Settings.ACTION_NOTIFICATION_LISTENER_SETTINGS))
                return
            }
            isServiceEnabled = true
            EventLog.add("감시 시작됨")
        } else {
            isServiceEnabled = false
            EventLog.add("감시 중지됨")
        }
        refreshStatus()
    }

    private fun requestBluetoothPermissions() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
            requestBluetoothPermission.launch(Manifest.permission.BLUETOOTH_CONNECT)
        }
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
            requestNotificationPermission.launch(Manifest.permission.POST_NOTIFICATIONS)
        }
    }

    private fun hasBluetoothConnectPermission(): Boolean {
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.S) return true
        return ContextCompat.checkSelfPermission(
            this, Manifest.permission.BLUETOOTH_CONNECT
        ) == PackageManager.PERMISSION_GRANTED
    }

    private fun isNotificationListenerEnabled(): Boolean {
        return NotificationManagerCompat.getEnabledListenerPackages(this).contains(packageName)
    }

    private fun showDevicePicker() {
        if (!hasBluetoothConnectPermission()) {
            requestBluetoothPermissions()
            return
        }

        val devices = BudsConnector.getBondedDevices(this)
        if (devices.isEmpty()) {
            AlertDialog.Builder(this)
                .setTitle(R.string.select_device_title)
                .setMessage(R.string.no_paired_devices)
                .setPositiveButton(android.R.string.ok, null)
                .show()
            return
        }

        val names = devices.map { BudsConnector.displayName(it) }.toTypedArray()
        AlertDialog.Builder(this)
            .setTitle(R.string.select_device_title)
            .setItems(names) { _, index ->
                val device = devices[index]
                targetDeviceAddress = device.address
                targetDeviceName = BudsConnector.displayName(device)
                EventLog.add("대상 기기 선택됨: ${targetDeviceName} (${device.address})")
                refreshStatus()
            }
            .show()
    }
}
