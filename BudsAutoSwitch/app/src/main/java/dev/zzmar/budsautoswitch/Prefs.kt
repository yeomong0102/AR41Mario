package dev.zzmar.budsautoswitch

import android.content.Context

object Prefs {
    private const val FILE = "buds_auto_switch_prefs"
    private const val KEY_DEVICE_ADDRESS = "device_address"
    private const val KEY_DEVICE_NAME = "device_name"
    private const val KEY_SERVICE_ENABLED = "service_enabled"

    private fun prefs(context: Context) =
        context.getSharedPreferences(FILE, Context.MODE_PRIVATE)

    var Context.targetDeviceAddress: String?
        get() = prefs(this).getString(KEY_DEVICE_ADDRESS, null)
        set(value) = prefs(this).edit().putString(KEY_DEVICE_ADDRESS, value).apply()

    var Context.targetDeviceName: String?
        get() = prefs(this).getString(KEY_DEVICE_NAME, null)
        set(value) = prefs(this).edit().putString(KEY_DEVICE_NAME, value).apply()

    var Context.isServiceEnabled: Boolean
        get() = prefs(this).getBoolean(KEY_SERVICE_ENABLED, false)
        set(value) = prefs(this).edit().putBoolean(KEY_SERVICE_ENABLED, value).apply()
}
