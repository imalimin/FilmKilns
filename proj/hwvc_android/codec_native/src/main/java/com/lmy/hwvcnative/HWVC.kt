package com.lmy.hwvcnative

import android.content.Context
import com.lmy.hwvcnative.tools.AlJavaRuntime

class HWVC {
    companion object {
        @Synchronized
        fun init(context: Context) {
            System.loadLibrary("hwffmpeg")
            System.loadLibrary("yuv")
            System.loadLibrary("al_common")
            System.loadLibrary("al_bitmap")
            System.loadLibrary("al_graphic")
            System.loadLibrary("al_codec")
            System.loadLibrary("al_image")
            System.loadLibrary("al_media")
            AlJavaRuntime.init()
        }
    }
}