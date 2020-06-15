package com.lmy.hwvcnative.processor

import com.lmy.hwvcnative.CPPObject

class AlVideoV2Processor : CPPObject() {
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

    private external fun create(): Long
    private external fun release(handle: Long)
    private external fun addTrack(handle: Long, type: Int, path: String)
}

object MediaType {
    const val TYPE_VIDEO = 0
    const val TYPE_AUDIO = 1
}