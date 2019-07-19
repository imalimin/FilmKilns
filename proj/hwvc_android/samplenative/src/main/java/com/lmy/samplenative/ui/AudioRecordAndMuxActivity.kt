package com.lmy.samplenative.ui

import android.content.Intent
import com.lmy.hwvcnative.processor.HwMicrophoneProcessor
import com.lmy.samplenative.BaseActivity
import com.lmy.samplenative.R
import kotlinx.android.synthetic.main.activity_audio_record_and_mux.*
import java.io.File

class AudioRecordAndMuxActivity : BaseActivity() {
    private val recorder = HwMicrophoneProcessor()
    private var recording = false
    override fun getLayoutResource() = R.layout.activity_audio_record_and_mux

    override fun initView() {
        recorder.prepare(File(externalCacheDir, "pcm.wav").path)
        recordBtn.setOnClickListener {
            recording = !recording
            if (recording) {
                recorder.start()
                recordBtn.text = "START"
            } else {
                recorder.pause()
                recordBtn.text = "PAUSE"
            }
        }
        nextBtn.setOnClickListener {
            //            startActivity(Intent(this,))
            finish()
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        recorder.release()
    }
}