package com.lmy.samplenative

import android.content.Intent
import android.net.Uri
import android.os.Environment
import android.text.TextUtils
import android.util.Log
import android.view.KeyEvent
import android.widget.SeekBar
import android.widget.Toast
import com.lmy.hwvcnative.processor.AudioProcessor
import kotlinx.android.synthetic.main.activity_audio.*
import java.io.File
import java.text.SimpleDateFormat
import java.util.*

class AudioActivity : BaseActivity(), SeekBar.OnSeekBarChangeListener {
    private var processor: AudioProcessor? = null
    private val format = SimpleDateFormat("mm:ss")
    private var playing: Boolean = true

    override fun getLayoutResource(): Int = R.layout.activity_audio
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
        processor = lastCustomNonConfigurationInstance as AudioProcessor?
        if (null == processor) {
            processor = AudioProcessor()
            processor?.setSource(path!!)
            processor?.prepare()
            processor?.start()
        }
        seekBar.setOnSeekBarChangeListener(this)
        processor?.setOnPlayProgressListener { us, duration ->
            timeView.text = format.format(Date(us / 1000))
            seekBar.progress = (us * 100 / duration).toInt()
        }
        playBtn.setOnClickListener {
            if (playing) {
                playBtn.setImageResource(android.R.drawable.ic_media_play)
                processor?.pause()
            } else {
                playBtn.setImageResource(android.R.drawable.ic_media_pause)
                processor?.start()
            }
            playing = !playing
        }
    }

    override fun onResume() {
        super.onResume()
        processor?.start()
    }

    override fun onPause() {
        super.onPause()
        processor?.pause()
    }

    override fun onKeyDown(keyCode: Int, event: KeyEvent?): Boolean {
        if (keyCode == KeyEvent.KEYCODE_BACK) {
            Log.i("HWVC", "VideoActivity back key pressed.")
            processor?.stop()
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