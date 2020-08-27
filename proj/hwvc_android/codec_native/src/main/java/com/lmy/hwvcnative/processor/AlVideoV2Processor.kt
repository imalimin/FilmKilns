package com.lmy.hwvcnative.processor

import android.graphics.PointF
import android.os.Handler
import android.os.Looper
import android.view.Surface
import androidx.annotation.Keep
import com.lmy.hwvcnative.CPPObject
import com.lmy.hwvcnative.entity.AlMediaTrack
import com.lmy.hwvcnative.entity.AlParcel
import com.lmy.hwvcnative.entity.AlRational
import com.lmy.hwvcnative.entity.AlResult

@Keep
class AlVideoV2Processor : CPPObject() {
    private var onPlayProgressListener: ((timeInUS: Long, duration: Long) -> Unit)? = null
    private var onTrackUpdateListener: ((AlMediaTrack) -> Unit)? = null
    private val mHandler = Handler(Looper.getMainLooper())

    init {
        handler = create()
    }

    fun release() {
        if (!isNativeNull()) {
            release(handler)
            handler = 0
        }
    }

    fun addTrack(
        type: Int,
        path: String,
        seqInInUS: Long,
        trimInInUS: Long = 0,
        durationInUS: Long = 0
    ): Int {
        if (!isNativeNull()) {
            return addTrack(
                handler,
                type,
                path,
                seqInInUS,
                seqInInUS + durationInUS,
                trimInInUS,
                trimInInUS + durationInUS
            )
        }
        return -1
    }

    fun removeTrack(trackID: Int) {
        if (!isNativeNull()) {
            removeTrack(handler, trackID)
        }
    }

    fun start() {
        if (!isNativeNull()) {
            start(handler)
        }
    }

    fun pause() {
        if (!isNativeNull()) {
            pause(handler)
        }
    }

    fun seek(timeInUS: Long) {
        if (!isNativeNull()) {
            seek(handler, timeInUS)
        }
    }

    fun setOnPlayProgressListener(l: (timeInUS: Long, duration: Long) -> Unit) {
        onPlayProgressListener = l
    }

    fun setOnTrackUpdateListener(l: (AlMediaTrack) -> Unit) {
        onTrackUpdateListener = l
    }

    fun updateWindow(surface: Surface) {
        if (!isNativeNull()) {
            updateWindow(handler, surface)
        }
    }

    fun setCanvasBackground(type: Int) {
        if (!isNativeNull()) {
            setCanvasBackground(handler, type)
        }
    }

    fun getLayer(x: Float, y: Float): Int {
        if (!isNativeNull()) {
            return getLayer(handler, x, y)
        }
        return AlResult.FAILED
    }

    fun postTranslate(id: Int, dx: Float, dy: Float): Int {
        if (!isNativeNull()) {
            return postTranslate(handler, id, dx, dy)
        }
        return AlResult.FAILED
    }

    fun postScale(id: Int, ds: AlRational, anchor: PointF): Int {
        if (!isNativeNull()) {
            return postScale(handler, id, ds.num, ds.den, anchor.x, anchor.y)
        }
        return AlResult.FAILED
    }

    private fun onDispatchNativeMessage(what: Int, arg0: Int, arg1: Long, arg2: Long) {
//        Log.d("AlVideoV2Processor", "$what, $arg0, $arg1, $arg2")
        when (what) {
            0 -> {
                mHandler.post {
                    onPlayProgressListener?.invoke(arg1, arg2)
                }
            }
        }
    }

    private fun onNativeTrackUpdate(data: ByteArray) {
        if (data.isNotEmpty()) {
            mHandler.post {
                onTrackUpdateListener?.invoke(AlMediaTrack(AlParcel.from(data)))
            }
        }
    }

    private external fun create(): Long
    private external fun release(handle: Long)
    private external fun addTrack(
        handle: Long,
        type: Int,
        path: String,
        seqIn: Long,
        seqOut: Long,
        trimIn: Long,
        trimOut: Long
    ): Int

    private external fun removeTrack(handle: Long, trackID: Int)
    private external fun start(handle: Long)
    private external fun pause(handle: Long)
    private external fun seek(handle: Long, timeInUS: Long)
    private external fun updateWindow(handle: Long, surface: Surface)
    private external fun setCanvasBackground(handle: Long, type: Int)
    private external fun getLayer(handler: Long, x: Float, y: Float): Int
    private external fun postTranslate(handler: Long, id: Int, dx: Float, dy: Float): Int
    private external fun postScale(handler: Long, id: Int, dsNum: Int, dsDen: Int,
                                   anchorX: Float, anchorY: Float): Int
}