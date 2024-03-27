package com.lmy.common.ext

import android.graphics.Color
import androidx.appcompat.widget.Toolbar
import androidx.core.view.ViewCompat

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