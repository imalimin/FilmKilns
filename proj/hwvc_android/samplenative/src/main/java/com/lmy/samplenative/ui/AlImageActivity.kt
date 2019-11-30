package com.lmy.samplenative.ui

import android.app.Activity
import android.content.Intent
import android.os.Environment
import android.util.Log
import android.view.KeyEvent
import android.view.SurfaceHolder
import android.view.View
import android.widget.SeekBar
import com.lmy.common.ui.GallerySelectActivity
import com.lmy.hwvcnative.processor.AlImageProcessor
import com.lmy.samplenative.BaseActivity
import com.lmy.samplenative.R
import kotlinx.android.synthetic.main.activity_al_image.*
import java.io.File

class AlImageActivity : BaseActivity(), SeekBar.OnSeekBarChangeListener, View.OnClickListener {
    private var processor: AlImageProcessor? = null
    private val mLayers = ArrayList<Int>()
    private var mCurrentLayer = -1
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
        surfaceView.keepScreenOn = true
        surfaceView.holder.addCallback(surfaceCallback)
        GallerySelectActivity.request(this, 100, 1)
        addLayerBtn.setOnClickListener(this)
        scaleBar.setOnSeekBarChangeListener(this)
        transBar.setOnSeekBarChangeListener(this)
        processor = lastCustomNonConfigurationInstance as AlImageProcessor?
        if (null == processor) {
            processor = AlImageProcessor.create()
        }
        processor?.setCanvas(1080, 1920, 0)
    }

    override fun onClick(v: View?) {
        when (v?.id) {
            R.id.addLayerBtn -> {
                GallerySelectActivity.request(this, 100, 1)
//                processor?.removeLayer(mCurrentLayer)
            }
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
                processor?.setRotation(mCurrentLayer, 3.141592653f * progress / 1000)
            R.id.transBar ->
                processor?.setTranslate(mCurrentLayer, progress / 1000f, progress / 1000f)
        }
    }

    override fun onStartTrackingTouch(seekBar: SeekBar?) {
    }

    override fun onStopTrackingTouch(seekBar: SeekBar?) {
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (Activity.RESULT_OK != resultCode) {
            return
        }
        when (requestCode) {
            100 -> {
                val result = GallerySelectActivity.getResultDtata(data)
                if (null != result && result.isNotEmpty()) {
                    val layerId = processor?.addLayer(result[0])
                    if (null != layerId && layerId >= 0) {
                        mLayers.add(layerId)
                        mCurrentLayer = layerId
                        Log.i("HWVC", "addLayer $layerId")
                    }
                }
            }
        }
    }
}