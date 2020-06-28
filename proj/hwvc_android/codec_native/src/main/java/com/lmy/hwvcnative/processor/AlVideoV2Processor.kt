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
    ) {
        if (!isNativeNull()) {
            addTrack(handler, type, path, seqInInUS, seqOutInUS, trimInInUS, trimOutInUS)
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
    )

    private external fun start(handle: Long)
    private external fun pause(handle: Long)
}

object MediaType {
    const val TYPE_VIDEO = 0
    const val TYPE_AUDIO = 1
}