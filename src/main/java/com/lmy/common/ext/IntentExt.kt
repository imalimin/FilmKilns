package com.lmy.common.ext

import android.content.Context
import android.content.Intent
import android.net.Uri
import android.os.Build
import android.support.v4.content.FileProvider
import java.io.File

fun Intent.fromFile(context: Context, file: File): Intent {
    action = Intent.ACTION_VIEW
    addCategory("android.intent.category.DEFAULT")
    addFlags(Intent.FLAG_ACTIVITY_NEW_TASK)
    val uri = if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
        addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION)
        FileProvider.getUriForFile(context, "${context.packageName}.fileProvider", file)
    } else {
        Uri.fromFile(file)
    }
    setDataAndType(uri, file.getMIMEType())
    return this
}