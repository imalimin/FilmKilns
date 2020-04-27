package com.lmy.hwvcnative.core

import android.media.MediaCodec
import android.util.Log

open class AlMediaCodec() {
    private var codec: MediaCodec? = null

    companion object {
        fun create(): AlMediaCodec = AlMediaCodec()
    }

    init {
        Log.i("AlMediaCodec", "alloc")
    }

    fun write(tex: Int, w: Int, h: Int) {

    }
}