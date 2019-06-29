package com.lmy.hwvcnative.processor

import com.lmy.hwvcnative.CPPObject

class HwCameraRecorder : CPPObject() {
    init {
        handler = create()
    }

    fun prepare() {
        if (0L == handler) return
        prepare(handler)
    }

    fun release() {
        if (0L == handler) return
        release(handler)
        handler = 0L
    }

    private external fun create(): Long
    private external fun prepare(handler: Long)
    private external fun release(handler: Long)
}