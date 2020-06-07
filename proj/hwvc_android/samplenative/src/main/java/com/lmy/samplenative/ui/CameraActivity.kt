package com.lmy.samplenative.ui

import android.annotation.SuppressLint
import android.content.Intent
import android.net.Uri
import android.os.AsyncTask
import android.util.Log
import android.view.SurfaceHolder
import com.lmy.hwvcnative.entity.AlAudioParams
import com.lmy.hwvcnative.entity.AlVideoParams
import com.lmy.hwvcnative.processor.AlCameraRecorder
import com.lmy.samplenative.BaseActivity
import com.lmy.samplenative.FilterController
import com.lmy.samplenative.R
import com.lmy.samplenative.VideoActivity
import kotlinx.android.synthetic.main.activity_camera.*
import kotlinx.android.synthetic.main.activity_camera.surfaceView
import kotlinx.android.synthetic.main.activity_camera.timeView
import kotlinx.android.synthetic.main.view_filter.*
import java.io.File
import java.text.SimpleDateFormat
import java.util.*

class CameraActivity : BaseActivity() {
    private lateinit var mFilterController: FilterController
    private var recorder: AlCameraRecorder? = null
    private var recording = false
    private var requestPreview = false
    private lateinit var path: String
    private val formator = SimpleDateFormat("mm:ss.SSS")

    override fun getLayoutResource(): Int = R.layout.activity_camera
    override fun initView() {
        surfaceView.fitsSystemWindows = true
        if (isNightMode()) {
            optLayout.setBackgroundColor(resources.getColor(R.color.black_232))
        }
        path = "${externalCacheDir!!.path}/camera.mp4"
        recorder = AlCameraRecorder(applicationContext, AlVideoParams(720, 1280), AlAudioParams())
        recorder?.setOutputFilePath(path)
        recorder?.setOnRecordProgressListener {
            timeView.text = formator.format(Date(it / 1000))
        }
        mFilterController = FilterController(recorder!!, progressLayout)
        filterBtn.setOnClickListener {
            mFilterController.chooseFilter(this)
        }
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
        backBtn.setOnClickListener {
            recorder?.backward()
        }
        swapBtn.setOnClickListener {
            recorder?.swapCamera()
        }
        surfaceView.keepScreenOn = true
        surfaceView.holder.addCallback(object : SurfaceHolder.Callback {
            override fun surfaceChanged(holder: SurfaceHolder, p1: Int, p2: Int, p3: Int) {
                recorder?.updateWindow(holder.surface)
            }

            override fun surfaceDestroyed(p0: SurfaceHolder?) {
                Log.i("HWVC", "surfaceDestroyed")
            }

            override fun surfaceCreated(holder: SurfaceHolder) {
            }
        })
    }

    override fun onDestroy() {
        super.onDestroy()
        if (!requestPreview) {
            recorder?.release()
        }
    }
}