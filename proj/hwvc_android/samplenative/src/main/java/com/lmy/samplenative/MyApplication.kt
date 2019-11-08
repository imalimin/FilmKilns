/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
package com.lmy.samplenative

import android.app.Application
import com.lmy.hwvcnative.HWVC
import com.tencent.bugly.crashreport.CrashReport

/**
 * Created by aliminabc@gmail.com on 2018/5/29.
 */
class MyApplication : Application() {
    override fun onCreate() {
        super.onCreate()
        CrashReport.initCrashReport(applicationContext, "d7f93990a9", false)
        HWVC.init(applicationContext)
    }
}