package com.lmy.common.ext

import android.content.Context

fun Float.toPx(context: Context) = Math.round(context.resources.displayMetrics.density * this)
fun Float.toSp(context: Context) = this / Math.round(context.resources.displayMetrics.scaledDensity) + 0.5f