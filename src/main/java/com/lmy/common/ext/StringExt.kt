package com.lmy.common.ext

import android.text.TextUtils
import com.lmy.common.utils.AESCrypt
import com.lmy.common.utils.MD5
import com.lmy.common.utils.Validator
import java.net.URLEncoder
import java.text.SimpleDateFormat

fun String.fileName(): String? = if (TextUtils.isEmpty(this))
    null
else
    this.substring(this.lastIndexOf('/') + 1, this.length)


fun String.getTimeSinceNow(): String {
    val format = SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss")
    var ms = System.currentTimeMillis()
    try {
        ms = format.parse(this).time
    } catch (e: Exception) {
        e.printStackTrace()
    }
    val delta = ((System.currentTimeMillis() - ms) / 1000).toInt()
    return when {
        delta < 60 -> "刚刚"
        delta < 3600 -> String.format("%d分钟前", delta / 60)
        delta < 86400 -> String.format("%d小时前", delta / 3600)
        delta < 2592000 -> String.format("%d天前", delta / 86400)
        delta < 31536000 -> String.format("%d个月前", delta / 2592000)
        else -> String.format("%d年前", delta / 31536000)
    }
}

fun String.md5(): String = MD5.getMD5Code(this)!!

fun String.encrypt(key: String): String? = AESCrypt.encrypt(this, key)

fun String.urlEncode(): String = URLEncoder.encode(this)
fun String.isMobile(): Boolean = Validator.isMobile(this)
fun String.isPassword(): Boolean = Validator.isPassword(this)