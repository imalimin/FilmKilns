package com.alimin.fk.app.ui

import com.alimin.fk.app.R
import com.alimin.fk.module.FkModuleTest
import com.lmy.common.ui.BaseActivity

class ModuleTestActivity : BaseActivity() {
    override val layoutResID: Int = R.layout.activity_module_test

    override fun initView() {
        FkModuleTest().testAll()
    }
}