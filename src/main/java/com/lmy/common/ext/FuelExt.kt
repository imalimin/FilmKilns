package com.lmy.common.ext

import android.net.Uri
import android.text.TextUtils

private const val SUFFIX = "VRGgvbieSDlFeaXm"
fun String.withSign(): String {
    val uri = Uri.parse(this)
    val segments = uri.pathSegments
    if (null == segments || 0 == segments.size) {
        return this
    }
    val map = HashMap<String, String>()
    uri.queryParameterNames.forEach {
        //检查保留字
        if (TextUtils.equals(it.toLowerCase(), "sign")) {
            throw RuntimeException("Invalid sign")
        }
        map[it] = uri.getQueryParameter(it)!!
    }
    val hasStamp = map.containsKey("timestamp")
    if (!hasStamp) {
        map["timestamp"] = System.currentTimeMillis().toString()
    }
    val sb = StringBuffer()
    map.toSortedMap().forEach {
        if (it.value.length <= 16 && TextUtils.equals(it.key, "sign")) {
            sb.append(it.value)
        }
    }
    sb.append(SUFFIX)
    return if (hasStamp) {
        "$this&sign=${sb.toString().md5()}"
    } else {
        "$this&sign=${sb.toString().md5()}&timestamp=${map["timestamp"]}"
    }
}