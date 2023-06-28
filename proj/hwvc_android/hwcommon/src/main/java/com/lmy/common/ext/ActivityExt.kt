package com.lmy.common.ext

import android.app.Activity
import android.os.Build
import android.text.TextUtils
import android.util.TypedValue
import android.view.View
import android.view.ViewGroup
import android.view.WindowManager
import androidx.appcompat.app.AppCompatActivity
import androidx.appcompat.widget.Toolbar
import androidx.core.app.ActivityCompat
import com.lmy.common.R


fun Activity.disableStatusBarPadding() {
    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
        window.decorView.systemUiVisibility = (window.decorView.systemUiVisibility
                or View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                or View.SYSTEM_UI_FLAG_LAYOUT_STABLE)
    }
}

fun Activity.disableNavigationBarPadding() {
    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
        window.addFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_NAVIGATION)
    }
}

fun Activity.setLightMode(dark: Boolean) {
    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
        setSystemUiVisibility(getSystemUiVisibility() and View.SYSTEM_UI_FLAG_LIGHT_STATUS_BAR.inv())//清除标记
        addSystemUiVisibilityFlag(if (dark) View.SYSTEM_UI_FLAG_LIGHT_STATUS_BAR else 0)
    }
}

fun Activity.getColorPrimary(): Int {
    val typedValue = TypedValue()
    theme.resolveAttribute(R.attr.colorPrimaryDark, typedValue, true)
    return typedValue.data
}

/**
 * 获取Uri中的参数
 *
 * @param key
 * @return
 */
fun Activity.getQueryParamsString(key: String): String? {
    val intent = intent ?: return null
    val uri = intent.data ?: return null
    val segments = uri.pathSegments
    return if (null == segments || 0 == segments.size) null else uri.getQueryParameter(key)
}

/**
 * 获取Uri中的参数
 *
 * @param key
 * @param def
 * @return
 */
fun Activity.getQueryParamsInt(key: String, def: Int): Int {
    val str = getQueryParamsString(key)
    return if (TextUtils.isEmpty(str) || !TextUtils.isDigitsOnly(str)) def else Integer.parseInt(str!!)
}

fun Activity.addSystemUiVisibilityFlag(flag: Int) {
    setSystemUiVisibility(getSystemUiVisibility() or flag)
}

private fun Activity.getSystemUiVisibility(): Int {
    return window.decorView.systemUiVisibility
}

private fun Activity.setSystemUiVisibility(visibility: Int) {
    window.decorView.systemUiVisibility = visibility
}

fun AppCompatActivity.attachToolbar(title: Int):
        Toolbar? = attachToolbar(title, android.R.drawable.ic_menu_close_clear_cancel)

fun AppCompatActivity.attachToolbar(title: Int, navIcon: Int): Toolbar? {
    val toolbar = Toolbar(this)
    var contentView: View = window.decorView.findViewById(android.R.id.content)
    if (contentView is ViewGroup) {
        contentView = contentView.getChildAt(0)
        if (contentView is ViewGroup) {
            contentView.addView(toolbar, 0)
        } else {
            return null
        }
    } else {
        return null
    }
    toolbar.setTitle(title)
    setSupportActionBar(toolbar)
    toolbar.setNavigationIcon(navIcon)
    toolbar.setNavigationOnClickListener { ActivityCompat.finishAfterTransition(this) }
    return toolbar
}

fun Activity.setResultOk() {
    setResult(Activity.RESULT_OK)
}