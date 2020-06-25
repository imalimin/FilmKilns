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

    fun addTrack(type: Int, path: String) {
        if (!isNativeNull()) {
            addTrack(handler, type, path)
        }
    }

    fun start() {
        if (!isNativeNull()) {
            start(handler)
        }
    }

    fun stop() {
        if (!isNativeNull()) {
            stop(handler)
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
    private external fun addTrack(handle: Long, type: Int, path: String)
    private external fun start(handle: Long)
    private external fun stop(handle: Long)
}

object MediaType {
    const val TYPE_VIDEO = 0
    const val TYPE_AUDIO = 1
}