package com.lmy.common.ext

import java.text.SimpleDateFormat
import java.util.*

val FORMAT_DEFAULT = SimpleDateFormat("yyyy-MM-dd HH:mm:ss")
fun Date.getFormatTime(): String {
    return FORMAT_DEFAULT.format(this.time)
}