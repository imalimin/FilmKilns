package com.lmy.hwvcnative.processor

import android.util.Log
import com.lmy.hwvcnative.CPPObject

class AlVideoV2Processor : CPPObject() {
    private var onPlayProgressListener: ((timeInUS: Long, duration: Long) -> Unit)? = null

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

    private fun onDispatchNativeMessage(what: Int, arg0: Int, arg1: Long, arg2: Long) {
        Log.i("AlVideoV2Processor", "$what, $arg0, $arg1, $arg2")
        when (what) {
            0 -> {
                onPlayProgressListener?.invoke(arg1, arg2)
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
    private external fun removeTrack(handle: Long,trackID: Int)
    private external fun start(handle: Long)
    private external fun pause(handle: Long)
    private external fun seek(handle: Long, timeInUS: Long)
}

object MediaType {
    const val TYPE_VIDEO = 0
    const val TYPE_AUDIO = 1
}