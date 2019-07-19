package com.lmy.hwvcnative.processor

import com.lmy.hwvcnative.CPPObject

class HwMicrophoneProcessor : CPPObject() {

    init {
        handler = create()
    }

    fun prepare(filePath: String) {
        if (!isNativeNull()) {
            prepare(handler, filePath)
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

    @Synchronized
    fun release() {
        if (!isNativeNull()) {
            release(handler)
        }
        handler = 0
    }

    private external fun create(): Long
    private external fun prepare(handler: Long, filePath: String)
    private external fun start(handler: Long)
    private external fun pause(handler: Long)
    private external fun release(handler: Long)
}