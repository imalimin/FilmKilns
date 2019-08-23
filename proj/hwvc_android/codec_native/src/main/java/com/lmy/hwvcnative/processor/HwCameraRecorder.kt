package com.lmy.hwvcnative.processor

import android.graphics.SurfaceTexture
import android.util.Log
import android.view.Surface
import android.view.SurfaceHolder
import android.view.SurfaceView
import com.lmy.hwvcnative.CPPObject
import com.lmy.hwvcnative.FilterSupport
import com.lmy.hwvcnative.devices.CameraWrapper
import com.lmy.hwvcnative.filter.Filter

class HwCameraRecorder : CPPObject(), FilterSupport, SurfaceTexture.OnFrameAvailableListener {
    private var filter: Filter? = null
    private var camera: CameraWrapper? = null
    private var prepared = false

    init {
        handler = create()
    }

    fun setOutputFilePath(filePath: String) {
        if (0L == handler) return
        setOutputFilePath(handler, filePath)
    }

    fun setFormat(width: Int, height: Int, sampleFormat: Int = 102,
                  channels: Int = 2, sampleRate: Int = 44100) {
        if (0L == handler) return
        setFormat(handler, width, height, sampleFormat, channels, sampleRate)
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

    fun prepare(view: SurfaceView) {
        view.holder.addCallback(object : SurfaceHolder.Callback {
            override fun surfaceChanged(holder: SurfaceHolder, p1: Int, p2: Int, p3: Int) {
                if (!prepared) {
                    prepared = true
                    prepare(holder.surface)
                    postEvent(handler, 1)
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

    fun start() {
        if (0L == handler) return
        start(handler)
    }

    fun pause() {
        if (0L == handler) return
        pause(handler)
    }

    override fun onFrameAvailable(surfaceTexture: SurfaceTexture) {
        if (0L == handler) return
        postEvent(handler, 3)
    }

    fun onHandleMessage(what: Int, arg1: Int) {
//        Log.i("CameraActivity", "onHandleMessage $what")
        when (what) {
            1 -> camera = CameraWrapper.open(arg1, this)
            2 -> camera?.release()
            3 -> {
                camera?.draw()
                if (0L != handler) {
                    invalidate(handler, camera!!.getMatrix(), 0,
                            CameraWrapper.VIDEO_WIDTH, CameraWrapper.VIDEO_HEIGHT,
                            camera!!.cameraHeight, camera!!.cameraWidth)
                }
            }
        }
    }

    override fun setFilter(filter: Filter) {
        if (isNativeNull()) return
        this.filter = filter
        setFilter(handler, filter.handler)
    }

    override fun getFilter(): Filter? {
        return this.filter
    }

    override fun invalidate() {
    }

    private external fun create(): Long
    private external fun prepare(handler: Long, surface: Surface)
    private external fun updateWindow(handler: Long, surface: Surface)
    private external fun start(handler: Long)
    private external fun pause(handler: Long)
    private external fun release(handler: Long)
    private external fun postEvent(handler: Long, what: Int)
    private external fun invalidate(handler: Long, matrix: FloatArray, tsInNs: Long,
                                    w: Int, h: Int, cw: Int, ch: Int)

    private external fun setOutputFilePath(handler: Long, filePath: String)
    private external fun setFormat(handler: Long, width: Int, height: Int, sampleFormat: Int,
                                   channels: Int, sampleRate: Int)

    private external fun setFilter(handler: Long, filter: Long)
}