package com.lmy.common.ext

import android.content.Context
import android.view.KeyCharacterMap
import android.view.KeyEvent
import android.view.View
import android.view.ViewConfiguration

fun Float.toPx(context: Context) = Math.round(context.resources.displayMetrics.density * this)
fun Float.toSp(context: Context) = this / Math.round(context.resources.displayMetrics.scaledDensity) + 0.5f
fun View.locationInWindow(): IntArray {
    val location = IntArray(2)
    getLocationInWindow(location)//获取View在窗体的坐标
    return location
}

fun Context.getStatusBarHeight(): Int {
    var result = 0
    val resourceId = resources.getIdentifier("status_bar_height", "dimen", "android")
    if (resourceId > 0) {
        result = resources.getDimensionPixelSize(resourceId)
    }
    return result
}

fun Context.hasNavigationBar(): Boolean {
    //通过判断设备是否有返回键、菜单键(不是虚拟键,是手机屏幕外的按键)来确定是否有navigation bar
    val hasMenuKey = ViewConfiguration.get(this).hasPermanentMenuKey()
    val hasBackKey = KeyCharacterMap.deviceHasKey(KeyEvent.KEYCODE_BACK)
    return !hasMenuKey && !hasBackKey
}

fun Context.getNavigationBarHeight(): Int {
    if (!hasNavigationBar()) return 0
    val resourceId = resources.getIdentifier("navigation_bar_height", "dimen", "android")
    return resources.getDimensionPixelSize(resourceId)
}

fun Context.deviceScreenSize(): IntArray {
    val outMetrics = resources.displayMetrics
    return intArrayOf(outMetrics.widthPixels, outMetrics.heightPixels)
}