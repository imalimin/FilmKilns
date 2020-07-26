package com.lmy.samplenative.ui

import android.content.Intent
import android.text.TextUtils
import android.util.Log
import android.view.SurfaceHolder
import android.widget.Toast
import androidx.recyclerview.widget.LinearLayoutManager
import com.lmy.hwvcnative.entity.AlLayer
import com.lmy.hwvcnative.entity.AlMediaTrack
import com.lmy.hwvcnative.entity.AlMediaType
import com.lmy.hwvcnative.processor.AlVideoV2Processor
import com.lmy.hwvcnative.tools.AlFFUtils
import com.lmy.hwvcnative.widget.AlTrackContainer
import com.lmy.samplenative.BaseActivity
import com.lmy.samplenative.R
import com.lmy.samplenative.adapter.AlTrackAdapter
import kotlinx.android.synthetic.main.activity_video_v2.*
import kotlinx.android.synthetic.main.activity_video_v2.surfaceView
import java.text.SimpleDateFormat
import java.util.*
import kotlin.collections.ArrayList

class AlVideoV2Activity : BaseActivity() {
    override fun getLayoutResource(): Int = R.layout.activity_video_v2
    private val processor: AlVideoV2Processor? = AlVideoV2Processor()
    private val fmt = SimpleDateFormat("mm:ss")
    private var mCurrentLayer = AlLayer.none()
    private var playing: Boolean = true
    private var duration: Long = -1
    private val mAdapter = AlTrackAdapter()
    private val surfaceCallback = object : SurfaceHolder.Callback {
        override fun surfaceChanged(holder: SurfaceHolder, p1: Int, p2: Int, p3: Int) {
            processor?.updateWindow(holder.surface)
        }

        override fun surfaceDestroyed(p0: SurfaceHolder?) {
            Log.i("AlVideoV2Activity", "surfaceDestroyed")
        }

        override fun surfaceCreated(holder: SurfaceHolder) {
            Log.i("AlVideoV2Activity", "surfaceCreated")
        }
    }

    override fun initView() {
        var uri = intent.data
        if (uri == null)
            uri = intent.getParcelableExtra(Intent.EXTRA_STREAM)
        if (uri == null) {
            finish()
            return
        }
        val path = getRealFilePath(uri)
        if (TextUtils.isEmpty(path)) {
            Toast.makeText(this, "File NOT exist.", Toast.LENGTH_SHORT).show()
            finish()
            return
        }
        setupView()
        surfaceView.keepScreenOn = true
        surfaceView.holder.addCallback(surfaceCallback)
        processor?.setOnPlayProgressListener { timeInUS, duration ->
            onPlayProgress(timeInUS, duration)
        }
        processor?.setOnTrackUpdateListener {
            onTrackUpdate(it)
        }
        val info = AlFFUtils.trackInfo(path!!)
        if (info and 0x1 > 0) {
            addTrack(path!!, AlMediaType.TYPE_VIDEO)
        }
        if (info and 0x2 > 0) {
            addTrack(path!!, AlMediaType.TYPE_AUDIO)
        }
        processor?.start()
    }

    private fun addTrack(path: String, type: Int) {
        val id = processor?.addTrack(type, path, 0)
        if (null != id && id >= 0) {
            mAdapter.items?.add(AlMediaTrack(id, type))
            trackView.addTrack(AlMediaTrack(id, type))
        }
    }

    private fun onPlayProgress(timeInUS: Long, duration: Long) {
        this.duration = duration
        trackView.setDuration(duration)
        trackView.setProgress(timeInUS / duration.toFloat())
        timeView.text =
            "${fmt.format(Date(timeInUS / 1000))}/${fmt.format(Date(duration / 1000))}"
    }

    private fun onTrackUpdate(track: AlMediaTrack) {
        trackView.updateTrack(track)
    }

    private fun setupView() {
        mAdapter.bindData(ArrayList<AlMediaTrack>())
        recyclerView.layoutManager = LinearLayoutManager(this)
        recyclerView.adapter = mAdapter
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
        addBtn.setOnClickListener {
            addTrack("/sdcard/duet_input.mp4", AlMediaType.TYPE_VIDEO)
            addTrack("/sdcard/the-world-today.m4a", AlMediaType.TYPE_AUDIO)
        }
        trackView.setOnSeekBarChangeListener(object : AlTrackContainer.OnSeekBarChangeListener {
            override fun onProgressChanged(progress: Float, fromUser: Boolean) {
                if (fromUser) {
                    processor?.seek((duration * progress).toLong())
                }
            }

            override fun onStartTrackingTouch() {
                processor?.pause()
            }

            override fun onStopTrackingTouch() {
                processor?.start()
            }
        })
        surfaceView?.setOnClickListener { v, x, y ->
            if (null != processor) {
                setCurLayer(AlLayer(processor.getLayer(x, y), 0, 0))
            }
        }
        surfaceView.setOnScrollListener { v, x, y, dx, dy, s ->
            processor?.postTranslate(getCurrentLayer().id, dx, dy)
        }
        surfaceView?.setOnScaleListener { v, ds, anchor ->
            processor?.postScale(getCurrentLayer().id, ds, anchor)
        }
    }

    fun setCurLayer(layer: AlLayer) {
        mCurrentLayer = layer
    }

    fun getCurrentLayer(): AlLayer = mCurrentLayer

    override fun onDestroy() {
        super.onDestroy()
        processor?.pause()
        processor?.release()
    }
}