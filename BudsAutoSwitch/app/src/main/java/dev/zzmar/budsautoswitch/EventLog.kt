package dev.zzmar.budsautoswitch

import android.os.Handler
import android.os.Looper
import java.text.SimpleDateFormat
import java.util.Locale

/**
 * In-memory, process-wide log of recent actions, surfaced in MainActivity
 * so the user can see whether auto-connect attempts actually happened.
 */
object EventLog {
    private const val MAX_LINES = 100
    private val lines = ArrayDeque<String>()
    private val listeners = mutableListOf<(String) -> Unit>()
    private val mainHandler = Handler(Looper.getMainLooper())
    private val timeFormat = SimpleDateFormat("HH:mm:ss", Locale.getDefault())

    @Synchronized
    fun add(message: String) {
        val line = "${timeFormat.format(System.currentTimeMillis())}  $message"
        lines.addLast(line)
        while (lines.size > MAX_LINES) lines.removeFirst()
        val snapshot = currentTextLocked()
        mainHandler.post {
            listeners.toList().forEach { it(snapshot) }
        }
    }

    @Synchronized
    fun currentText(): String = currentTextLocked()

    private fun currentTextLocked(): String = lines.joinToString("\n")

    fun addListener(listener: (String) -> Unit) {
        listeners.add(listener)
    }

    fun removeListener(listener: (String) -> Unit) {
        listeners.remove(listener)
    }
}
