package com.lmy.common.ui

import android.os.Build
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import android.view.View
import android.view.WindowManager
import androidx.appcompat.widget.Toolbar
import androidx.core.app.ActivityCompat
import androidx.viewbinding.ViewBinding
import com.lmy.common.ext.addSystemUiVisibilityFlag

abstract class BaseActivity : AppCompatActivity() {
    protected abstract val layoutResID: Int
    protected abstract fun initView()
    protected open fun getViewBinding(): ViewBinding? = null
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        val viewBinding = getViewBinding()
        if (viewBinding != null) {
            setContentView(viewBinding.root)
        } else {
            setContentView(layoutResID)
        }
        initView()
    }

    fun showToolbar(toolbar: Toolbar, tile: Int, navIcon: Int) {
        showToolbar(toolbar, resources.getString(tile), navIcon)
    }

    fun showToolbar(toolbar: Toolbar, tile: String, navIcon: Int) {
        toolbar.title = tile
        setSupportActionBar(toolbar)
        toolbar.setNavigationIcon(navIcon)
        toolbar.setNavigationOnClickListener { ActivityCompat.finishAfterTransition(this@BaseActivity) }
    }

    fun setDarkStatusBar() {
        setLightStatusBar(true)
    }

    fun setLightStatusBar(dark: Boolean) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            setSystemUiVisibility(getSystemUiVisibility() and View.SYSTEM_UI_FLAG_LIGHT_STATUS_BAR.inv())//清除标记
            addSystemUiVisibilityFlag(if (dark) View.SYSTEM_UI_FLAG_LIGHT_STATUS_BAR else 0)
        }
    }

    fun fillStatusBar() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            addSystemUiVisibilityFlag(View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN or View.SYSTEM_UI_FLAG_LAYOUT_STABLE)
        }
    }

    fun fillNavigation() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
            window.addFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_NAVIGATION)
        }
    }

    private fun getSystemUiVisibility(): Int = window.decorView.systemUiVisibility

    private fun setSystemUiVisibility(visibility: Int) {
        window.decorView.systemUiVisibility = visibility
    }
}