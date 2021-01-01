/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
package com.alimin.fk.app

import com.alimin.fk.FilmKilns
import com.lmy.common.BaseApplication

/**
 * Created by aliminabc@gmail.com on 2018/5/29.
 */
class MyApplication : BaseApplication() {
    override fun onCreate() {
        super.onCreate()
//        CrashReport.initCrashReport(applicationContext, "d7f93990a9", false)
        FilmKilns.init(applicationContext)
    }
}