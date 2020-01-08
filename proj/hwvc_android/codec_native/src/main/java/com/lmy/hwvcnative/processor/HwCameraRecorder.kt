package com.lmy.hwvcnative.processor

import android.graphics.SurfaceTexture
import android.os.Handler
import android.os.Looper
import android.util.Log
import android.view.Surface
import android.view.SurfaceHolder
import android.view.SurfaceView
import com.lmy.hwvcnative.CPPObject
import com.lmy.hwvcnative.FilterSupport
import com.lmy.hwvcnative.devices.CameraWrapper
import com.lmy.hwvcnative.filter.Filter
import java.lang.RuntimeException

class HwCameraRecorder : CPPObject(), FilterSupport, SurfaceTexture.OnFrameAvailableListener {
    private var filter: Filter? = null
    private var camera: CameraWrapper? = null
    private var prepared = false
    private var onRecordProgressListener: ((Long) -> Unit)? = null
    private val mHandler = Handler(Looper.getMainLooper())
    private var mCameraIndex = CameraWrapper.CameraIndex.FRONT
    private var videoWidth = 544
    private var videoHeight = 960

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
        videoWidth = width
        videoHeight = height
        setFormat(handler, videoWidth, videoHeight, sampleFormat, channels, sampleRate)
    }

    fun release() {
        if (0L == handler) return
        postEvent(handler, EVENT_RELEASE)
        release(handler)
        handler = 0L
    }

    fun swapCamera() {
        if (0L == handler) return
        postEvent(handler, EVENT_SWAP)
    }

    fun prepare() {
        if (0L == handler) return
        postEvent(handler, EVENT_PREPARE)
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
        postEvent(handler, EVENT_DRAW)
    }

    fun onHandleMessage(what: Int, arg1: Int) {
//        Log.i("CameraActivity", "onHandleMessage $what")
        when (what) {
            1 -> camera = CameraWrapper.open(mCameraIndex, videoWidth, videoHeight,
                    arg1, this)
            2 -> camera?.release()
            4 -> {
                mCameraIndex = if (CameraWrapper.CameraIndex.FRONT == mCameraIndex)
                    CameraWrapper.CameraIndex.BACK
                else {
                    CameraWrapper.CameraIndex.FRONT
                }
                camera?.switchCamera(mCameraIndex)
            }
            3 -> {
                camera?.draw()
                if (0L != handler) {
                    invalidate(handler, camera!!.getMatrix(), camera!!.timestamp(),
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

    fun updateWindow(surface: Surface) {
        if (isNativeNull()) return
        updateWindow(handler, surface)
    }

    override fun getFilter(): Filter? {
        return this.filter
    }

    override fun invalidate() {
    }

    fun backward() {
        if (isNativeNull()) return
        backward(handler)
    }

    fun setOnRecordProgressListener(listener: (Long) -> Unit) {
        this.onRecordProgressListener = listener
    }

    /**
     * Don`t touch. Call from jni.
     * @param timeInUs Timestamp for record progress.
     */
    fun onRecordProgress(timeInUs: Long) {
        mHandler.post {
            onRecordProgressListener?.invoke(timeInUs)
        }
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

    private external fun setFilter(handler: Long, filter: Long)
    private external fun backward(handler: Long)

    companion object {
        const val EVENT_PREPARE = 1
        const val EVENT_RELEASE = 2
        const val EVENT_DRAW = 3
        const val EVENT_SWAP = 4
    }
}