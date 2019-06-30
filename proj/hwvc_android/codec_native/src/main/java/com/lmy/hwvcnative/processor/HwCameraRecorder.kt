package com.lmy.hwvcnative.processor

import android.graphics.SurfaceTexture
import android.util.Log
import android.view.Surface
import android.view.SurfaceHolder
import android.view.SurfaceView
import com.lmy.hwvcnative.CPPObject
import com.lmy.hwvcnative.devices.CameraWrapper

class HwCameraRecorder : CPPObject(), SurfaceTexture.OnFrameAvailableListener {
    private var camera: CameraWrapper? = null
    private var prepared = false

    init {
        handler = create()
    }

    private fun prepare(surface: Surface) {
        if (0L == handler) return
        prepare(handler, surface)
    }

    fun release() {
        if (0L == handler) return
        postEvent(handler, 2)
        release(handler)
        handler = 0L
    }

    fun startPreview(view: SurfaceView) {
        view.holder.addCallback(object : SurfaceHolder.Callback {
            override fun surfaceChanged(holder: SurfaceHolder, p1: Int, p2: Int, p3: Int) {
                if (!prepared) {
                    prepared = true
                    postEvent(handler, 1)
                    prepare(holder.surface)
                } else {
                    if (0L != handler) {
                        updateWindow(handler, holder.surface)
                    }
                }
            }

            override fun surfaceDestroyed(p0: SurfaceHolder?) {
                Log.i("HWVC", "surfaceDestroyed")
            }

            override fun surfaceCreated(holder: SurfaceHolder) {
            }
        })
    }

    override fun onFrameAvailable(surfaceTexture: SurfaceTexture) {
        Log.i("CameraActivity", "onFrameAvailable")
        if (0L == handler) return
        postEvent(handler, 3)
    }

    fun onHandleMessage(what: Int) {
        Log.i("CameraActivity", "onHandleMessage $what")
        when (what) {
            1 -> camera = CameraWrapper(this)
            2 -> camera?.release()
            3 -> {
                val textures = camera?.draw()
                if (0L != handler) {
                    invalidate(handler, textures!![0], 720, 1280)
                }
            }
        }
    }

    private external fun create(): Long
    private external fun prepare(handler: Long, surface: Surface)
    private external fun updateWindow(handler: Long, surface: Surface)
    private external fun release(handler: Long)
    private external fun postEvent(handler: Long, what: Int)
    private external fun invalidate(handler: Long, textureId: Int, w: Int, h: Int)
}