package com.lmy.common

import android.app.Application
import com.facebook.drawee.backends.pipeline.Fresco

open class BaseApplication : Application() {
    override fun onCreate() {
        super.onCreate()
        Fresco.initialize(baseContext)
    }
}