package com.lmy.samplenative.ui

import com.lmy.hwvcnative.processor.AlVideoV2Processor
import com.lmy.hwvcnative.processor.MediaType
import com.lmy.samplenative.BaseActivity
import com.lmy.samplenative.R
import kotlinx.android.synthetic.main.activity_video_v2.*
import java.text.SimpleDateFormat
import java.util.*

class AlVideoV2Activity : BaseActivity() {
    private val fmt = SimpleDateFormat("mm:ss")
    private val processor: AlVideoV2Processor? = AlVideoV2Processor()
    override fun getLayoutResource(): Int = R.layout.activity_video_v2
    override fun initView() {
        processor?.addTrack(MediaType.TYPE_AUDIO, "/sdcard/duet_input.wav")
        processor?.setOnPlayProgressListener { timeInUS, duration ->
            runOnUiThread {
                seekBar.progress = (timeInUS * 100 / duration).toInt()
                timeView.text = "${fmt.format(Date(timeInUS / 1000))}/${fmt.format(Date(duration / 1000))}"
            }
        }
        playBtn.setOnClickListener {
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        processor?.release()
    }
}