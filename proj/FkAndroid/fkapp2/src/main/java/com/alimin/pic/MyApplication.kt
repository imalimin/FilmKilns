/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
package com.alimin.pic

import com.alimin.fk.FilmKilns
import com.lmy.common.BaseApplication
import com.tencent.bugly.crashreport.CrashReport

/**
 * Created by aliminabc@gmail.com on 2018/5/29.
 */
class MyApplication : BaseApplication() {
    override fun onCreate() {
        super.onCreate()
        val strategy = CrashReport.UserStrategy(applicationContext)
        strategy.isEnableUserInfo = true
        CrashReport.initCrashReport(applicationContext, "d77740b753", BuildConfig.DEBUG, strategy)
        FilmKilns.init(applicationContext)
    }
}