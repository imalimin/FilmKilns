package com.lmy.samplenative

import android.content.Intent
import android.graphics.SurfaceTexture
import android.net.Uri
import android.os.Environment
import android.text.TextUtils
import android.util.Log
import android.view.KeyEvent
import android.view.Surface
import android.view.SurfaceHolder
import android.view.TextureView
import android.widget.SeekBar
import android.widget.Toast
import com.lmy.hwvcnative.processor.VideoProcessor
import kotlinx.android.synthetic.main.activity_video.*
import java.io.File

class VideoActivity : BaseActivity(), TextureView.SurfaceTextureListener,
        SeekBar.OnSeekBarChangeListener {

    private lateinit var mFilterController: FilterController
    private var processor: VideoProcessor? = null
    private var prepared = false
    private var surface: Surface? = null
    private var playing: Boolean = true

    override fun getLayoutResource(): Int = R.layout.activity_video
    override fun initView() {
        var uri = intent.data
        if (uri == null)
            uri = intent.getParcelableExtra(Intent.EXTRA_STREAM)
        if (uri == null) {
            val testFile = File(Environment.getExternalStorageDirectory(), "hw_test.mp4")
            if (!testFile.exists()) {
                Toast.makeText(this, "没有找到该文件", Toast.LENGTH_SHORT).show()
                finish()
                return
            }
            uri = Uri.fromFile(testFile)
        }
        val path = getRealFilePath(uri)
        if (TextUtils.isEmpty(path)) {
            Toast.makeText(this, "没有找到该文件", Toast.LENGTH_SHORT).show()
            finish()
            return
        }
        processor = lastCustomNonConfigurationInstance as VideoProcessor?
        if (null != processor) {
            prepared = true
        } else {
            processor = VideoProcessor()
            processor?.setSource(path!!)
            processor?.setOnPlayProgressListener { us ->
                seekBar.progress = (us * 100 / 177710867).toInt()
            }
        }
        mFilterController = FilterController(processor!!, progressLayout)
        filterBtn.setOnClickListener {
            mFilterController.chooseFilter(this)
        }
        seekBar.setOnSeekBarChangeListener(this)
        playBtn.setOnClickListener {
            if (playing) {
                processor?.pause()
            } else {
                processor?.start()
            }
            playing = !playing
        }
        surfaceView.keepScreenOn = true
        surfaceView.holder.addCallback(object : SurfaceHolder.Callback {
            override fun surfaceChanged(holder: SurfaceHolder, p1: Int, p2: Int, p3: Int) {
                if (!prepared) {
                    prepared = true
                    processor?.prepare(holder.surface)
                    processor?.start()
                } else {
                    processor?.updateWindow(holder.surface)
                }
            }

            override fun surfaceDestroyed(p0: SurfaceHolder?) {
                Log.i("HWVC", "surfaceDestroyed")
            }

            override fun surfaceCreated(holder: SurfaceHolder) {
            }
        })
//        surfaceView.surfaceTextureListener = this
    }

    override fun onRetainCustomNonConfigurationInstance(): Any? {
        Log.i("HWVC", "VideoActivity onRetainCustomNonConfigurationInstance")
        return processor
    }

    override fun onSurfaceTextureSizeChanged(surface: SurfaceTexture?, width: Int, height: Int) {
    }

    override fun onSurfaceTextureUpdated(surface: SurfaceTexture?) {
    }

    override fun onSurfaceTextureDestroyed(surface: SurfaceTexture?): Boolean {
//                processor?.release()
//                processor = null
        this.surface?.release()
        return true
    }

    override fun onSurfaceTextureAvailable(surface: SurfaceTexture, width: Int, height: Int) {
        this.surface = Surface(surface)
        processor?.prepare(this.surface!!)
    }

    override fun onResume() {
        super.onResume()
        processor?.start()
    }

    override fun onPause() {
        super.onPause()
        processor?.pause()
    }

    override fun onDestroy() {
        super.onDestroy()
        Log.i("HWVC", "VideoActivity onDestroy.")
    }

    override fun onKeyDown(keyCode: Int, event: KeyEvent?): Boolean {
        if (keyCode == KeyEvent.KEYCODE_BACK) {
            Log.i("HWVC", "VideoActivity back key pressed.")
            processor?.release()
            processor = null
        }
        return super.onKeyDown(keyCode, event)
    }

    override fun onProgressChanged(seekBar: SeekBar, progress: Int, fromUser: Boolean) {
        if (fromUser) {
            processor?.seek(177710867 * progress.toLong() / 100)
        }
    }

    override fun onStartTrackingTouch(seekBar: SeekBar) {
        processor?.pause()
    }

    override fun onStopTrackingTouch(seekBar: SeekBar) {
        processor?.start()
    }
}