package com.alimin.fk

import android.content.Context

class FilmKilns {
    companion object {
        @Synchronized
        fun init(context: Context?) {
            System.loadLibrary("skia")
            System.loadLibrary("protobuf")
            System.loadLibrary("FkBase")
            System.loadLibrary("FkFramework")
            System.loadLibrary("FkGraphic")
            System.loadLibrary("FkImage")
            System.loadLibrary("FkAndroid")
        }
    }
}