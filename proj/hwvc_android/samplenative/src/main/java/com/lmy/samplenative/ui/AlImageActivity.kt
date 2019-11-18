package com.lmy.samplenative.ui

import android.os.Environment
import com.lmy.hwvcnative.processor.AlImageProcessor
import com.lmy.samplenative.BaseActivity
import com.lmy.samplenative.R
import java.io.File

class AlImageActivity : BaseActivity() {
    private val processor = AlImageProcessor.create()
    override fun getLayoutResource(): Int = R.layout.activity_al_image
    override fun initView() {
        processor.addLayer(File(Environment.getExternalStorageDirectory(), "001.8.png").absolutePath)
    }
}