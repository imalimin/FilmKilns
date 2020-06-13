package com.lmy.samplenative.ui

import com.lmy.hwvcnative.processor.AlVideoV2Processor
import com.lmy.samplenative.BaseActivity
import com.lmy.samplenative.R

class AlVideoV2Activity : BaseActivity() {
    private val processor: AlVideoV2Processor? = AlVideoV2Processor()
    override fun getLayoutResource(): Int = R.layout.activity_video_v2
    override fun initView() {

    }

    override fun onDestroy() {
        super.onDestroy()
        processor?.release()
    }
}