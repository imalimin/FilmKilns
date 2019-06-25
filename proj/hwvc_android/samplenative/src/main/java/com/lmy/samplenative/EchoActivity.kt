package com.lmy.samplenative

import com.lmy.hwvcnative.media.HwEchoPlayer
import kotlinx.android.synthetic.main.activity_echo.*

class EchoActivity : BaseActivity() {
    private var echo: HwEchoPlayer? = null
    override fun getLayoutResource(): Int = R.layout.activity_echo
    override fun initView() {
        echo = HwEchoPlayer(applicationContext)
        echo?.start()
    }

    override fun onDestroy() {
        super.onDestroy()
        echo?.stop()
    }
}