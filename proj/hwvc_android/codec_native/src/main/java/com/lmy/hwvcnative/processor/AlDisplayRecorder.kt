package com.lmy.hwvcnative.processor

import android.graphics.SurfaceTexture
import android.os.Handler
import android.os.Looper
import android.util.Log
import android.view.Surface
import com.lmy.hwvcnative.CPPObject
import com.lmy.hwvcnative.devices.CameraWrapper

class AlDisplayRecorder : CPPObject(), SurfaceTexture.OnFrameAvailableListener {
    private val mHandler = Handler(Looper.getMainLooper())

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
        setFormat(handler, 544, 960, sampleFormat, channels, sampleRate)
    }

    fun start() {
        if (0L == handler) return
        start(handler)
    }

    fun pause() {
        if (0L == handler) return
        pause(handler)
    }

    fun release() {
        if (0L == handler) return
        postEvent(handler, EVENT_RELEASE)
        release(handler)
        handler = 0L
    }

    override fun onFrameAvailable(surfaceTexture: SurfaceTexture) {
        if (0L == handler) return
        postEvent(handler, EVENT_DRAW)
    }

    fun onHandleMessage(what: Int, arg1: Int) {
        when (what) {
//            EVENT_PREPARE ->
            HwCameraRecorder.EVENT_RELEASE -> camera?.release()
            HwCameraRecorder.EVENT_DRAW -> {
                camera?.draw()
                if (0L != handler) {
                    invalidate(handler, camera!!.getMatrix(), camera!!.timestamp(),
                            camera!!.cameraHeight, camera!!.cameraWidth)
                }
            }
        }
    }

    fun onNativePrepared(oesTex: Int) {
        if (null != camera || oesTex <= 0) {
            return
        }
        Log.i("HwCameraRecorder:", "OES tex $oesTex")
        camera = CameraWrapper.open(mCameraIndex, videoWidth, videoHeight,
                oesTex, this)
    }

    private external fun create(): Long
    private external fun updateWindow(handler: Long, surface: Surface)
    private external fun start(handler: Long)
    private external fun pause(handler: Long)
    private external fun release(handler: Long)
    private external fun postEvent(handler: Long, what: Int)
    private external fun invalidate(handler: Long, matrix: FloatArray, tsInNs: Long,
                                    cw: Int, ch: Int)

    private external fun setOutputFilePath(handler: Long, filePath: String)
    private external fun setFormat(handler: Long, width: Int, height: Int, sampleFormat: Int,
                                   channels: Int, sampleRate: Int)

    companion object {
        const val EVENT_PREPARE = 1
        const val EVENT_RELEASE = 2
        const val EVENT_DRAW = 3
        const val EVENT_SWAP = 4
    }
}