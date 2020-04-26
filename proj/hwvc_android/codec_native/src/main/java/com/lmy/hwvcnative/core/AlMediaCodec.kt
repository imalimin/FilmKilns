package com.lmy.hwvcnative.core

import android.media.MediaCodec

class AlMediaCodec {
    private var codec: MediaCodec? = null

    companion object {
        fun create(): AlMediaCodec = AlMediaCodec()
    }

    fun write(tex: Int, w: Int, h: Int) {

    }
}