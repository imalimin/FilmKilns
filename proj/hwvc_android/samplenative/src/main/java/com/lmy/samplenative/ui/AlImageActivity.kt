package com.lmy.samplenative.ui

import android.os.Environment
import android.util.Log
import android.view.SurfaceHolder
import com.lmy.hwvcnative.processor.AlImageProcessor
import com.lmy.samplenative.BaseActivity
import com.lmy.samplenative.R
import kotlinx.android.synthetic.main.activity_al_image.*
import java.io.File

class AlImageActivity : BaseActivity() {
    private val processor = AlImageProcessor.create()
    private var prepared = false
    private val surfaceCallback = object : SurfaceHolder.Callback {
        override fun surfaceChanged(holder: SurfaceHolder, p1: Int, p2: Int, p3: Int) {
            Log.i("HWVC", "surfaceChanged: $p1, $p2, $p3 | ${surfaceView.width}, ${surfaceView.height}")
            if (!prepared) {
                prepared = true
                processor.prepare(holder.surface)
                processor.addLayer(File(Environment.getExternalStorageDirectory(), "001.8.png").absolutePath)
            } else {
                processor.updateWindow(holder.surface)
            }
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
    }

    override fun onDestroy() {
        super.onDestroy()
        processor.release()
        surfaceView.holder.removeCallback(surfaceCallback)
    }
}