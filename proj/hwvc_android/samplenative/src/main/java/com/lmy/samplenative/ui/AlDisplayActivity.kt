package com.lmy.samplenative.ui

import android.content.Context
import android.content.Intent
import android.media.projection.MediaProjection
import android.media.projection.MediaProjectionManager
import android.net.Uri
import android.os.Build
import android.provider.Settings
import android.provider.Settings.ACTION_MANAGE_OVERLAY_PERMISSION
import android.util.Log
import android.view.SurfaceHolder
import com.lmy.hwvcnative.processor.AlDisplayRecorder
import com.lmy.samplenative.BaseActivity
import com.lmy.samplenative.R
import com.lmy.samplenative.VideoActivity
import kotlinx.android.synthetic.main.activity_display.*
import java.io.File
import java.text.SimpleDateFormat
import java.util.*


class AlDisplayActivity : BaseActivity() {
    private var recorder: AlDisplayRecorder? = null
    private lateinit var path: String
    private var mpm: MediaProjectionManager? = null
    private var mp: MediaProjection? = null
    private val format = SimpleDateFormat("mm:ss.SSS")
    private var recording = false
    private var requestPreview = false

    override fun getLayoutResource(): Int = R.layout.activity_display
    override fun initView() {
        mpm = getSystemService(Context.MEDIA_PROJECTION_SERVICE) as MediaProjectionManager
        startActivityForResult(mpm?.createScreenCaptureIntent(), REQ_PROJECTION)
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
            startActivity(Intent(this@AlDisplayActivity, VideoActivity::class.java).apply {
                data = Uri.fromFile(File(path))
            })
            finish()
            recorder?.release()
        }
        backBtn.setOnClickListener {
            //            recorder?.backward()
        }
        swapBtn.setOnClickListener {
            //            recorder?.swapCamera()
        }
    }

    private fun setup(mp: MediaProjection) {
        val metrics = resources.displayMetrics
        path = "${externalCacheDir.path}/camera.mp4"
        recorder = AlDisplayRecorder(mp, metrics.widthPixels, metrics.heightPixels,
                metrics.densityDpi)
        recorder?.setOutputFilePath(path)
        recorder?.setFormat(1088, 1792)
        recorder?.setOnRecordProgressListener {
            timeView.text = format.format(Date(it / 1000))
        }

        surfaceView.keepScreenOn = true
        if (null != surfaceView.holder.surface) {
            recorder?.showAt(surfaceView.holder.surface)
            return
        }
        surfaceView.holder.addCallback(object : SurfaceHolder.Callback {
            override fun surfaceChanged(holder: SurfaceHolder, p1: Int, p2: Int, p3: Int) {
                recorder?.showAt(holder.surface)
            }

            override fun surfaceDestroyed(p0: SurfaceHolder?) {
                Log.i("HWVC", "surfaceDestroyed")
            }

            override fun surfaceCreated(holder: SurfaceHolder) {
                Log.i("HWVC", "surfaceCreated")
            }
        })
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (RESULT_OK == resultCode) {
            if (data != null && REQ_PROJECTION == requestCode) {
                mp = mpm?.getMediaProjection(resultCode, data)
                if (null != mp) {
                    setup(mp!!)
                }
            }
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        if (!requestPreview) {
            recorder?.release()
        }
    }

    companion object {
        const val REQ_PROJECTION = 0x01
    }
}