package com.lmy.common.ext

import android.content.Context
import android.text.format.Formatter
import com.lmy.common.model.entity.MIMEType
import java.io.File
import java.util.*


fun File.size(): Long {
    return if (this.isDirectory) {
        val files = this.listFiles()
        var size = 0L
        files.forEach {
            size += it.fileCount()
        }
        size
    } else {
        this.inputStream().use { input ->
            input.available().toLong()
        }
    }
}

fun File.size(context: Context): String = Formatter.formatShortFileSize(context, size())

fun File.lastModifiedTime(): String {
    val cal = Calendar.getInstance()
    cal.timeInMillis = lastModified()
    return cal.time.getFormatTime()
}

fun File.copy(target: File, notify: (curFile: String) -> Unit) {
    if (this.isDirectory) {
        if (!target.exists()) {
            if (!target.mkdirs())
                throw FileSystemException(file = this, other = target, reason = "Failed to create target directory.")
        }
        val files = this.list()
        files.forEach {
            val srcFile = File(this, it)
            val destFile = File(target, it)
            srcFile.copy(destFile, notify)
        }
    } else {
//        debug_i("${this.path} -> ${target.path}")
        notify(this.path)
        target.parentFile?.mkdirs()
        this.inputStream().use { input ->
            target.outputStream().use { output ->
                input.copyTo(output, 1024 * 8)
            }
        }
    }
}

fun File.move(target: File, notify: (curFile: String) -> Unit) {
    if (this.isDirectory) {
        if (!target.exists()) {
            if (!target.mkdirs())
                throw FileSystemException(file = this, other = target, reason = "Failed to create target directory.")
        }
        val files = this.list()
        files.forEach {
            val srcFile = File(this, it)
            val destFile = File(target, it)
            srcFile.move(destFile, notify)
        }
        this.delete()
    } else {
//        debug_i("move ${this.path} -> ${target.path}")
        notify(this.path)
        this.renameTo(target)
    }
}

fun File.delete(notify: (curFile: String) -> Unit) {
    if (this.isDirectory) {
        val files = this.listFiles()
        files.forEach {
            it.delete(notify)
        }
        this.delete()
    } else {
//        debug_i("delete ${this.path}")
        notify(this.path)
        this.delete()
    }
}

fun File.fileCount(): Int {
    return if (this.isDirectory) {
        val files = this.listFiles()
        var i = 0
        files.forEach {
            i += it.fileCount()
        }
        i
    } else {
        1
    }
}

fun File.suffix(): String {
    return if (isDirectory) ""
    else {
        val n = name
        n.substring(n.lastIndexOf("."), n.length)
    }
}

fun File.getMIMEType(): String {
    return MIMEType.map[suffix()] ?: return MIMEType.map[""]!!
}