package com.lmy.hwvcnative.core

import android.media.MediaCodec

class HwMediaCodec {
    private var codec: MediaCodec? = null

    companion object {
        fun create(): HwMediaCodec = HwMediaCodec()
    }

    fun write(tex: Int, w: Int, h: Int) {

    }
}