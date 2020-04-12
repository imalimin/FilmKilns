package com.lmy.common.log

import android.util.Log
import java.io.File
import java.io.FileWriter
import java.text.SimpleDateFormat
import java.util.*

class LogWriter(private val logFile: File) {
    private val format = SimpleDateFormat("yyyy-MM-DD HH:mm:ss.SSS")
    private val calendar = Calendar.getInstance()
    private val writer: FileWriter = FileWriter(logFile, true)
    fun v(log: String) {
        v(TAG, log)
    }

    fun v(tag: String, log: String) {
        Log.v(tag, log)
        write(tag, log, "V")
    }

    fun d(log: String) {
        d(TAG, log)
    }

    fun d(tag: String, log: String) {
        Log.d(tag, log)
        write(tag, log, "D")
    }

    fun i(log: String) {
        i(TAG, log)
    }

    fun i(tag: String, log: String) {
        Log.i(tag, log)
        write(tag, log, "I")
    }

    fun e(log: String) {
        e(TAG, log)
    }

    fun e(tag: String, log: String) {
        Log.e(tag, log)
        write(tag, log, "E")
    }

    fun w(log: String) {
        w(TAG, log)
    }

    fun w(tag: String, log: String) {
        Log.w(tag, log)
        write(tag, log, "W")
    }

    private fun write(tag: String, log: String, type: String) {
        calendar.timeInMillis = System.currentTimeMillis()
        writer.write("${format.format(calendar.time)} $type/$tag: $log\n")
    }

    fun flush() {
        writer.flush()
    }

    fun release() {
        flush()
        writer.close()
    }

    companion object {
        private const val TAG = "LogWriter"
    }
}