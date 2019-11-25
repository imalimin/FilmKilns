package com.lmy.hwvcnative.processor

import android.view.Surface
import com.lmy.hwvcnative.CPPObject

class AlImageProcessor private constructor() : CPPObject() {
    init {
        handler = create()
    }

    fun release() {
        if (!isNativeNull()) {
            release(handler)
        }
        handler = 0L
    }

    fun prepare(surface: Surface) {
        if (!isNativeNull()) {
            prepare(handler, surface)
        }
    }

    fun updateWindow(surface: Surface) {
        if (!isNativeNull()) {
            updateWindow(handler, surface)
        }
        invalidate()
    }

    fun invalidate() {
        if (!isNativeNull()) {
            invalidate(handler)
        }
    }

    fun setCanvas(w: Int, h: Int, color: Int = 0) {
        if (!isNativeNull()) {
            setCanvas(handler, w, h, color)
        }
    }

    fun addLayer(path: String): Int {
        if (!isNativeNull()) {
            return addLayer(handler, path)
        }
        return -1
    }

    private external fun create(): Long
    private external fun release(handler: Long)
    private external fun prepare(handler: Long, surface: Surface)
    private external fun updateWindow(handler: Long, surface: Surface)
    private external fun invalidate(handler: Long)
    private external fun setCanvas(handler: Long, w: Int, h: Int, color: Int)
    private external fun addLayer(handler: Long, path: String): Int

    companion object {
        fun create(): AlImageProcessor = AlImageProcessor()
    }
}