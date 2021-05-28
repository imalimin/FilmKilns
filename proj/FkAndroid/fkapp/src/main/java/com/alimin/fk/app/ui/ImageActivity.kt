package com.alimin.fk.app.ui

import android.util.Log
import android.view.SurfaceHolder
import com.alimin.fk.app.R
import com.alimin.fk.engine.FkImage
import com.alimin.fk.module.FkModuleTest
import com.lmy.common.ui.BaseActivity
import kotlinx.android.synthetic.main.activity_image.*

class ImageActivity : BaseActivity(), SurfaceHolder.Callback {
    override val layoutResID: Int = R.layout.activity_image
    private var engine: FkImage = FkImage()

    override fun initView() {
        FkModuleTest().testAll()
        engine.create()
        surfaceView.holder.addCallback(this)
    }

    override fun onDestroy() {
        super.onDestroy()
        engine.destroy()
    }

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        engine.stop()
        engine.detachFromSurface(holder.surface)
    }

    override fun surfaceCreated(holder: SurfaceHolder) {
        engine.start()
        engine.attachToSurface(holder.surface)
        val layer = engine.newLayerWithColor(512, 512, 255, 0, 0, 0)
        Log.i("FilmKilns", "newLayer: $layer")
    }
}