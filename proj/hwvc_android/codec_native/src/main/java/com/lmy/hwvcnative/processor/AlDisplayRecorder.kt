package com.lmy.hwvcnative.processor

import android.graphics.SurfaceTexture
import android.hardware.display.DisplayManager
import android.hardware.display.VirtualDisplay
import android.media.projection.MediaProjection
import android.os.Handler
import android.os.Looper
import android.util.Log
import android.view.Surface
import com.lmy.hwvcnative.CPPObject

class AlDisplayRecorder(private val mp: MediaProjection,
                        private val dw: Int,
                        private val dh: Int,
                        private val dpi: Int) : CPPObject(), SurfaceTexture.OnFrameAvailableListener {
    private val mHandler = Handler(Looper.getMainLooper())
    private var vd: VirtualDisplay? = null
    private var texture: SurfaceTexture? = null
    private var transformMatrix: FloatArray = FloatArray(16)
    private var onRecordProgressListener: ((Long) -> Unit)? = null
    private val callback = object : VirtualDisplay.Callback() {
        override fun onResumed() {
            Log.i("AlDisplayRecorder", "onResumed")
        }

        override fun onStopped() {
            Log.i("AlDisplayRecorder", "onStopped")
        }

        override fun onPaused() {
            Log.i("AlDisplayRecorder", "onPaused")
        }
    }

    init {
        handler = create()
    }

    private fun setup(oesTex: Int) {
        Log.i("AlDisplayRecorder", "setup $dw x $dh, $dpi")
        texture = SurfaceTexture(oesTex)
        texture?.setOnFrameAvailableListener(this)
        vd = mp.createVirtualDisplay("hwvc", dw, dh, dpi,
                DisplayManager.VIRTUAL_DISPLAY_FLAG_AUTO_MIRROR,
                Surface(texture), null, null)
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
            HwCameraRecorder.EVENT_RELEASE -> {
                vd?.release()
                mp.stop()
                Log.i("AlDisplayRecorder", "release")
                texture?.release()
            }
            HwCameraRecorder.EVENT_DRAW -> {
                val time = texture!!.timestamp
                Log.i("AlDisplayRecorder", "draw $time")
                texture?.updateTexImage()
                texture?.getTransformMatrix(transformMatrix)
                if (0L != handler) {
                    invalidate(handler, transformMatrix, time, dw, dh)
                }
            }
        }
    }

    fun onNativePrepared(oesTex: Int) {
        if (oesTex <= 0) {
            return
        }
        Log.i("AlDisplayRecorder", "OES tex $oesTex")
        setup(oesTex)
    }

    fun onRecordProgress(timeInUs: Long) {
        mHandler.post {
            onRecordProgressListener?.invoke(timeInUs)
        }
    }

    fun setOnRecordProgressListener(listener: (Long) -> Unit) {
        this.onRecordProgressListener = listener
    }

    fun showAt(surface: Surface) {
        if (isNativeNull()) return
        updateWindow(handler, surface)
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