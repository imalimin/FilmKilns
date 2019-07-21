package com.lmy.samplenative.ui

import android.annotation.SuppressLint
import android.content.Intent
import android.net.Uri
import android.os.AsyncTask
import com.lmy.hwvcnative.processor.HwMicrophoneProcessor
import com.lmy.samplenative.AudioActivity
import com.lmy.samplenative.BaseActivity
import com.lmy.samplenative.R
import kotlinx.android.synthetic.main.activity_audio_record_and_mux.*
import java.io.File

class AudioRecordAndMuxActivity : BaseActivity() {
    private val recorder = HwMicrophoneProcessor()
    private var recording = false
    private var requestPreview = false
    private lateinit var path: String

    override fun getLayoutResource() = R.layout.activity_audio_record_and_mux

    override fun initView() {
        path = "${externalCacheDir.path}/audio.wav"
        recorder.prepare(path)
        recordBtn.keepScreenOn = true
        recordBtn.setOnClickListener {
            recording = !recording
            if (recording) {
                recorder.start()
                recordBtn.text = "PAUSE"
            } else {
                recorder.pause()
                recordBtn.text = "START"
            }
        }
        nextBtn.setOnClickListener {
            requestPreview = true
            @SuppressLint("CI_StaticFieldLeak")
            val task = object : AsyncTask<Void, Void, Void?>() {
                override fun onPostExecute(result: Void?) {
                    super.onPostExecute(result)
                    startActivity(Intent(this@AudioRecordAndMuxActivity, AudioActivity::class.java).apply {
                        data = Uri.fromFile(File(path))
                    })
                    finish()
                }

                override fun doInBackground(vararg params: Void?): Void? {
                    recorder?.release()
                    return null
                }
            }
            task.execute()
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        if (!requestPreview) {
            recorder?.release()
        }
    }
}