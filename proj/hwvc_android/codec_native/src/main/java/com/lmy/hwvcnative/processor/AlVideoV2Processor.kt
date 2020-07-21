package com.lmy.hwvcnative.processor

import android.os.Handler
import android.os.Looper
import android.view.Surface
import androidx.annotation.Keep
import com.lmy.hwvcnative.CPPObject
import com.lmy.hwvcnative.entity.AlMediaTrack
import com.lmy.hwvcnative.entity.AlParcel

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
        seqOutInUS: Long = 0,
        trimInInUS: Long = 0,
        trimOutInUS: Long = 0
    ): Int {
        if (!isNativeNull()) {
            return addTrack(handler, type, path, seqInInUS, seqOutInUS, trimInInUS, trimOutInUS)
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
}