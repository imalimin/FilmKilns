package com.lmy.hwvcnative.processor

import android.content.Context
import android.graphics.SurfaceTexture
import android.hardware.camera2.CameraManager
import android.os.Handler
import android.os.Looper
import android.util.Log
import android.view.Surface
import androidx.annotation.Keep
import com.lmy.hwvcnative.CPPObject
import com.lmy.hwvcnative.FilterSupport
import com.lmy.hwvcnative.devices.AlAbsCamera
import com.lmy.hwvcnative.devices.AlCameraFactory
import com.lmy.hwvcnative.entity.AlAudioParams
import com.lmy.hwvcnative.entity.AlVideoParams
import com.lmy.hwvcnative.filter.Filter

@Keep
class AlCameraRecorder(
    private val ctx: Context,
    private var vParams: AlVideoParams,
    private var aParams: AlAudioParams
) : CPPObject(), FilterSupport, SurfaceTexture.OnFrameAvailableListener {
    private var filter: Filter? = null
    private var camera: AlAbsCamera? = null
    private var onRecordProgressListener: ((Long) -> Unit)? = null
    private val mHandler = Handler(Looper.getMainLooper())
    private var mCameraIndex = AlAbsCamera.CameraIndex.FRONT

    init {
        handler = create()
        if (!isNativeNull()) {
            setFormat(
                handler,
                vParams.width,
                vParams.height,
                aParams.format,
                aParams.channles,
                aParams.sampleRate
            )
            setVideoBitLevel(handler, vParams.bitLevel)
            setProfile(handler, vParams.profile)
            setPreset(handler, vParams.preset)
            setEnableHardware(handler, vParams.enableHardware)
        }
    }

    fun setOutputFilePath(filePath: String) {
        if (isNativeNull()) return
        setOutputFilePath(handler, filePath)
    }

    fun release() {
        if (isNativeNull()) return
        postEvent(handler, EVENT_RELEASE)
        release(handler)
        handler = 0L
    }

    fun swapCamera() {
        if (isNativeNull()) return
        postEvent(handler, EVENT_SWAP)
    }

    fun start() {
        if (isNativeNull()) return
        start(handler)
    }

    fun pause() {
        if (isNativeNull()) return
        pause(handler)
    }

    override fun onFrameAvailable(surfaceTexture: SurfaceTexture) {
        if (isNativeNull()) return
        postEvent(handler, EVENT_DRAW)
    }

    fun onHandleMessage(what: Int, arg1: Int) {
        when (what) {
//            EVENT_PREPARE ->
            EVENT_RELEASE -> camera?.release()
            EVENT_DRAW -> {
                camera?.draw()
                if (0L != handler) {
                    invalidate(
                        handler, camera!!.getMatrix(), camera!!.timestamp(),
                        camera!!.getCameraSize().x, camera!!.getCameraSize().y
                    )
                }
            }
            EVENT_SWAP -> {
                mCameraIndex = if (AlAbsCamera.CameraIndex.FRONT == mCameraIndex)
                    AlAbsCamera.CameraIndex.BACK
                else {
                    AlAbsCamera.CameraIndex.FRONT
                }
                camera?.switchCamera(mCameraIndex)
            }
        }
    }

    fun onNativePrepared(oesTex: Int) {
        if (null != camera || oesTex <= 0) {
            return
        }
        Log.i("AlCameraRecorder:", "OES tex $oesTex")
        camera = AlCameraFactory.create(
            ctx.getSystemService(Context.CAMERA_SERVICE) as CameraManager,
            mCameraIndex, vParams.width, vParams.height, oesTex, this
        )
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
    private external fun invalidate(
        handler: Long, matrix: FloatArray, tsInNs: Long,
        cw: Int, ch: Int
    )

    private external fun setOutputFilePath(handler: Long, filePath: String)
    private external fun setFormat(
        handler: Long, width: Int, height: Int, sampleFormat: Int,
        channels: Int, sampleRate: Int
    )

    private external fun setFilter(handler: Long, filter: Long)
    private external fun backward(handler: Long)
    private external fun setVideoBitLevel(handler: Long, level: Int)

    private external fun setProfile(handler: Long, profile: String)

    private external fun setPreset(handler: Long, preset: String)

    private external fun setEnableHardware(handler: Long, enable: Boolean)

    companion object {
        const val EVENT_PREPARE = 1
        const val EVENT_RELEASE = 2
        const val EVENT_DRAW = 3
        const val EVENT_SWAP = 4
    }
}