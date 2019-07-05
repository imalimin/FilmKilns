package com.lmy.samplenative.ui

import android.content.Intent
import android.net.Uri
import com.lmy.hwvcnative.processor.HwCameraRecorder
import com.lmy.samplenative.BaseActivity
import com.lmy.samplenative.R
import com.lmy.samplenative.VideoActivity
import kotlinx.android.synthetic.main.activity_camera.*
import java.io.File

class CameraActivity : BaseActivity() {
    private var recorder: HwCameraRecorder? = HwCameraRecorder()
    private var recording = false
    private var requestPreview = false

    override fun getLayoutResource(): Int = R.layout.activity_camera
    override fun initView() {
        surfaceView.fitsSystemWindows = true
        surfaceView.keepScreenOn = true
        recorder?.startPreview(surfaceView)
        recordBtn.setOnClickListener {
            recording = !recording
            if (recording) {
                recorder?.start()
                recordBtn.text = "Recording"
            } else {
                recorder?.pause()
                recordBtn.text = "Record"
            }
        }
        nextBtn.setOnClickListener {
            requestPreview = true
            finish()
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        recorder?.release()
        if (requestPreview) {
            startActivity(Intent(this, VideoActivity::class.java).apply {
                data = Uri.fromFile(File("/sdcard/hw_encoder.mp4"))
            })
        }
    }
}