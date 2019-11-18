package com.lmy.hwvcnative.processor

import com.lmy.hwvcnative.CPPObject

class AlImageProcessor private constructor() : CPPObject() {
    init {
        handler = create()
    }

    fun setCanvas(w: Int, h: Int, color: Int = 0) {
        if (!isNativeNull()) {
            setCanvas(handler, w, h, color)
        }
    }

    fun addLayer(path: String) {
        if (!isNativeNull()) {
            val ret = addLayer(handler, path)
        }
    }

    private external fun create(): Long
    private external fun setCanvas(handler: Long, w: Int, h: Int, color: Int)
    private external fun addLayer(handler: Long, path: String): Int

    companion object {
        fun create(): AlImageProcessor = AlImageProcessor()
    }
}