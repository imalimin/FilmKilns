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

    private external fun create(): Long
    private external fun release(handle: Long)
}