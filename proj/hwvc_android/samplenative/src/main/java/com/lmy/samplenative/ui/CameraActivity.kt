package com.lmy.samplenative.ui

import android.annotation.SuppressLint
import android.content.Intent
import android.net.Uri
import android.os.AsyncTask
import android.os.Environment
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
    private lateinit var path: String

    override fun getLayoutResource(): Int = R.layout.activity_camera
    override fun initView() {
        surfaceView.fitsSystemWindows = true
        surfaceView.keepScreenOn = true
        path = "${externalCacheDir.path}/camera.mp4"
        recorder?.setOutputFilePath(path)
        recorder?.setOutputSize(720, 1280)
        recorder?.prepare(surfaceView)
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
            @SuppressLint("CI_StaticFieldLeak")
            val task = object : AsyncTask<Void, Void, Void?>() {
                override fun onPostExecute(result: Void?) {
                    super.onPostExecute(result)
                    startActivity(Intent(this@CameraActivity, VideoActivity::class.java).apply {
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