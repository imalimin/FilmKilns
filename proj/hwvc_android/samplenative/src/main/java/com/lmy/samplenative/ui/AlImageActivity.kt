package com.lmy.samplenative.ui

import android.os.Environment
import android.util.Log
import android.view.KeyEvent
import android.view.SurfaceHolder
import android.widget.SeekBar
import com.lmy.hwvcnative.processor.AlImageProcessor
import com.lmy.samplenative.BaseActivity
import com.lmy.samplenative.R
import kotlinx.android.synthetic.main.activity_al_image.*
import java.io.File

class AlImageActivity : BaseActivity(), SeekBar.OnSeekBarChangeListener {
    private var processor: AlImageProcessor? = null
    private var curLayerId = -1
    private val surfaceCallback = object : SurfaceHolder.Callback {
        override fun surfaceChanged(holder: SurfaceHolder, p1: Int, p2: Int, p3: Int) {
            processor?.updateWindow(holder.surface)
        }

        override fun surfaceDestroyed(p0: SurfaceHolder?) {
            Log.i("HWVC", "surfaceDestroyed")
        }

        override fun surfaceCreated(holder: SurfaceHolder) {
            Log.i("HWVC", "surfaceCreated")
        }
    }

    override fun getLayoutResource(): Int = R.layout.activity_al_image

    override fun initView() {
        scaleBar.setOnSeekBarChangeListener(this)
        transBar.setOnSeekBarChangeListener(this)
        processor = lastCustomNonConfigurationInstance as AlImageProcessor?
        if (null == processor) {
            processor = AlImageProcessor.create()
        }
        surfaceView.keepScreenOn = true
        surfaceView.holder.addCallback(surfaceCallback)
        processor?.setCanvas(1080, 1920, 0)
        val layerId0 = processor?.addLayer(File(Environment.getExternalStorageDirectory(), "喵-result.jpg").absolutePath)
        val layerId = processor?.addLayer(File(Environment.getExternalStorageDirectory(), "001.8.png").absolutePath)
        if (null != layerId) {
            curLayerId = layerId
        }
        Log.i("HWVC", "addLayer $layerId")
        handleBtn.setOnClickListener {
            processor?.removeLayer(layerId0!!)
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        surfaceView.holder.removeCallback(surfaceCallback)
    }

    //横竖屏切换
    override fun onRetainCustomNonConfigurationInstance(): Any? {
        return processor
    }

    override fun onKeyDown(keyCode: Int, event: KeyEvent?): Boolean {
        if (keyCode == KeyEvent.KEYCODE_BACK) {
            processor?.release()
        }
        return super.onKeyDown(keyCode, event)
    }

    override fun onProgressChanged(seekBar: SeekBar, progress: Int, fromUser: Boolean) {
        when (seekBar.id) {
            R.id.scaleBar ->
                processor?.setRotation(curLayerId, 3.141592653f * progress / 1000)
            R.id.transBar ->
                processor?.setTranslate(curLayerId, progress / 1000f, progress / 1000f)
        }
    }

    override fun onStartTrackingTouch(seekBar: SeekBar?) {
    }

    override fun onStopTrackingTouch(seekBar: SeekBar?) {
    }
}