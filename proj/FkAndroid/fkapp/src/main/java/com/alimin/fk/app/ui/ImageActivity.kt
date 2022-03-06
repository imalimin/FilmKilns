package com.alimin.fk.app.ui

import android.Manifest
import android.graphics.PointF
import android.view.SurfaceHolder
import android.view.SurfaceView
import android.view.ViewGroup
import android.widget.FrameLayout
import com.alimin.fk.app.R
import com.alimin.fk.engine.FkImage
import com.alimin.fk.entity.FkRational
import com.alimin.fk.widgets.FkActSurfaceView
import com.lmy.common.ui.BaseActivity
import kotlinx.android.synthetic.main.activity_image.*
import pub.devrel.easypermissions.AfterPermissionGranted
import pub.devrel.easypermissions.EasyPermissions

class ImageActivity : BaseActivity(),
    SurfaceHolder.Callback,
    FkActSurfaceView.OnScrollListener,
    FkActSurfaceView.OnRotateListener,
    FkActSurfaceView.OnScaleListener,
    FkActSurfaceView.OnActionListener {
    override val layoutResID: Int = R.layout.activity_image
    private var surfaceView: FkActSurfaceView? = null
    private var engine: FkImage = FkImage()
    private var layer = -1

    @AfterPermissionGranted(REQ_PERMISSION)
    override fun initView() {
        val perms = arrayOf(
            Manifest.permission.WRITE_EXTERNAL_STORAGE
        )
        if (EasyPermissions.hasPermissions(this, *perms)) {
            engine.create()
            surfaceView = FkActSurfaceView(this)
            surfaceView?.holder?.addCallback(this)
            surfaceView?.setOnScrollListener(this)
            surfaceView?.setOnScaleListener(this)
            surfaceView?.setOnRotateListener(this)
            surfaceView?.setOnActionListener(this)
            val lp = FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT)
            surfaceHolder.addView(surfaceView, lp)
        } else {
            EasyPermissions.requestPermissions(
                this, "Request write sdcard permission", REQ_PERMISSION, *perms
            )
        }
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
        layer = engine.newLayerWithFile("/sdcard/000000.jpg")
//        engine.setCanvasSize(512, 512)
//        layer = engine.newLayerWithColor(512,512, 0,255,255, 255)
//        Log.i("FilmKilns", "newLayer: $layer")
//        engine.drawPoint(0, 0xff0000, 300, 300)
        engine.notifyRender()
    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<out String>,
        grantResults: IntArray
    ) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        EasyPermissions.onRequestPermissionsResult(requestCode, permissions, grantResults, this)
    }

    companion object {
        private const val REQ_PERMISSION = 123
    }

    override fun onScroll(v: SurfaceView, x: Float, y: Float, dx: Float, dy: Float, status: Int) {
        engine.postTranslate(layer, (dx * v.measuredWidth / 2).toInt(), (-dy * v.measuredHeight / 2).toInt())
    }

    override fun onRotate(v: SurfaceView, dr: FkRational, anchor: PointF) {
        engine.postRotation(layer, dr.num, dr.den)
    }

    override fun onScale(v: SurfaceView, ds: FkRational, anchor: PointF) {
        engine.postScale(layer, ds.num.toFloat() / ds.den, ds.num.toFloat() / ds.den)
    }

    override fun onRender(v: SurfaceView) {
        engine.notifyRender()
    }
}