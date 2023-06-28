package com.alimin.fk

import android.content.Context

class FilmKilns {
    companion object {
        @Synchronized
        fun init(context: Context?) {
            System.loadLibrary("skia")
            System.loadLibrary("protobuf")
            System.loadLibrary("yuv")
            System.loadLibrary("hwffmpeg")
            System.loadLibrary("FilmKilns")
            System.loadLibrary("FkAndroid")
        }
    }
}