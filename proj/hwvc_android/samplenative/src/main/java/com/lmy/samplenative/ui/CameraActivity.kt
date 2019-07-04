package com.lmy.samplenative.ui

import com.lmy.hwvcnative.processor.HwCameraRecorder
import com.lmy.samplenative.BaseActivity
import com.lmy.samplenative.R
import kotlinx.android.synthetic.main.activity_camera.*

class CameraActivity : BaseActivity() {
    private var recorder: HwCameraRecorder? = HwCameraRecorder()
    private var recording = false

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
    }

    override fun onDestroy() {
        super.onDestroy()
        recorder?.release()
    }
}