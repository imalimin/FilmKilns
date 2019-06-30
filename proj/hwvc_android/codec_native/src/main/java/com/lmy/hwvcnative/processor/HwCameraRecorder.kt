package com.lmy.hwvcnative.processor

import android.graphics.SurfaceTexture
import android.util.Log
import android.view.Surface
import android.view.TextureView
import com.lmy.hwvcnative.CPPObject
import com.lmy.hwvcnative.devices.CameraWrapper

class HwCameraRecorder : CPPObject(), SurfaceTexture.OnFrameAvailableListener {
    private var camera: CameraWrapper? = CameraWrapper(this)

    init {
        handler = create()
        postEvent(handler, 0)
    }

    fun prepare(surface: Surface) {
        if (0L == handler) return
        prepare(handler, surface)
    }

    fun release() {
        if (0L == handler) return
        camera?.release()
        release(handler)
        handler = 0L
    }

    fun startPreview(view: TextureView) {
        view.surfaceTextureListener = object : TextureView.SurfaceTextureListener {

            override fun onSurfaceTextureSizeChanged(p0: SurfaceTexture?, p1: Int, p2: Int) {
//                render?.updateSize(p1, p2)
            }

            override fun onSurfaceTextureUpdated(p0: SurfaceTexture?) {
            }

            override fun onSurfaceTextureDestroyed(p0: SurfaceTexture?): Boolean {
//                stop()
                return true
            }

            override fun onSurfaceTextureAvailable(p0: SurfaceTexture?, p1: Int, p2: Int) {
                if (null != p0) {
                    camera
//                    startPreview(p0, p1, p2)
                }
                Log.i("CameraActivity", "onSurfaceTextureAvailable")
            }
        }
    }

    override fun onFrameAvailable(surfaceTexture: SurfaceTexture?) {

    }

    fun onHandleMessage(what: Int) {
        Log.i("CameraActivity", "onHandleMessage $what")
    }

    private external fun create(): Long
    private external fun prepare(handler: Long, surface: Surface)
    private external fun release(handler: Long)
    private external fun postEvent(handler: Long, what: Int)
}