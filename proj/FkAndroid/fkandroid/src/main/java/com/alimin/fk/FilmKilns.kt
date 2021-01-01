package com.alimin.fk

import android.content.Context

class FilmKilns {
    companion object {
        @Synchronized
        fun init(context: Context?) {
            if (BuildConfig.ENABLE_TEST) {
//                System.loadLibrary("gtest")
//                System.loadLibrary("gtest_main")
                System.loadLibrary("FkAndroidTest")
            }
            System.loadLibrary("al_common")
            System.loadLibrary("FkBase")
            System.loadLibrary("FkFramework")
            System.loadLibrary("FkAndroid")
        }
    }
}