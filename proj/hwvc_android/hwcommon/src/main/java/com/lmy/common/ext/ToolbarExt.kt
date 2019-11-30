package com.lmy.common.ext

import android.graphics.Color
import android.support.v4.view.ViewCompat
import android.support.v7.widget.Toolbar

fun Toolbar.defaultStyle(): Toolbar {
    val elevation = Math.round(context.resources.displayMetrics.density * 3).toFloat()
    setBackgroundColor(Color.WHITE)
    ViewCompat.setElevation(this, elevation)
    return this
}

fun Toolbar.fit(): Toolbar {
    fitsSystemWindows = true
    return this
}