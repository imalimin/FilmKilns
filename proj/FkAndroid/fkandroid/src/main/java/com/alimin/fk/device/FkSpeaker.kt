package com.alimin.fk.device

import android.media.*
import android.os.Build
import android.os.Handler
import android.os.HandlerThread
import android.os.Message
import android.util.Log
import com.alimin.fk.entity.FkAudioSettings
import com.alimin.fk.entity.FkResult
import java.io.File
import java.io.FileInputStream
import java.nio.ByteBuffer

open class FkSpeaker : FkDevice() {
    companion object {
        const val TAG = "FkSpeaker"
    }

    var strategy: FkSyncStrategy? = null
    private lateinit var mTrack: AudioTrack
    private val timestamp = AudioTimestamp()
    private var settings: FkAudioSettings? = null
    private var mByteBuffer: ByteBuffer? = null
    private var minBufSize = 0
    private val mThread = HandlerThread(TAG)
    private var mHandler: Handler? = null
    private var mIsPlaying = false

    private var bis: FileInputStream? = null

    private fun handleMessage(msg: Message) {
        if (0 == msg.what) {
            android.os.Process.setThreadPriority(android.os.Process.THREAD_PRIORITY_URGENT_AUDIO)
            return
        }
        while (mIsPlaying && AudioTrack.PLAYSTATE_PLAYING == mTrack.playState) {
            try {
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                    val size = bis?.read(mByteBuffer!!.array(), 0, minBufSize)
                    mByteBuffer!!.rewind()
                    mTrack.write(mByteBuffer!!, size!!, AudioTrack.WRITE_BLOCKING)
                    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
                        if (strategy?.farTimestampInNano!! == Long.MIN_VALUE) {
                            mTrack.getTimestamp(timestamp)
                            if (0L != timestamp.nanoTime) {
                                strategy?.farTimestampInNano = timestamp.nanoTime
                            }
                        }
                    }
//                    Log.i(TAG, "Read: ${timestamp.nanoTime}/${System.nanoTime()}, ${timestamp.framePosition}, $size")
                }
            } catch (e: Exception) {
                e.printStackTrace()
            }
        }
        Log.i(TAG, "Stop play loop")
    }

    fun init(settings: FkAudioSettings, path: String): FkResult {
        Log.e(TAG, "Path: $path")
        val file = File(path)
        if (!file.exists()) {
            return FkResult.FK_FILE_NOT_FOUND
        }
        this.settings = settings
        val sampleRate = settings.sampleRate
        val channelConfig = if (1 == settings.channels) AudioFormat.CHANNEL_IN_MONO
        else AudioFormat.CHANNEL_IN_STEREO
        val audioFormat =
            when (settings.format) {
                8 -> AudioFormat.ENCODING_PCM_8BIT
                32 -> AudioFormat.ENCODING_PCM_FLOAT
                else -> AudioFormat.ENCODING_PCM_16BIT
            }
        minBufSize = AudioTrack.getMinBufferSize(sampleRate, channelConfig, audioFormat)
        mByteBuffer = ByteBuffer.allocateDirect(minBufSize)
        mTrack = AudioTrack(
            AudioManager.STREAM_MUSIC,
            sampleRate,
            channelConfig,
            audioFormat,
            minBufSize,
            AudioTrack.MODE_STREAM
        )

        try {
            bis = FileInputStream(file)
        } catch (e: Exception) {
            e.printStackTrace()
        }

        mThread.start()
        mHandler = object : Handler(mThread.looper) {
            override fun handleMessage(msg: Message) {
                this@FkSpeaker.handleMessage(msg)
            }
        }
        return FkResult.FK_OK
    }

    fun start(): FkResult {
        if (AudioTrack.STATE_INITIALIZED != mTrack.playState) {
            Log.e(TAG, "Invalid state: ${mTrack.playState}")
            return FkResult.FK_INVALID_STATE
        }
        try {
            mTrack.play()
            if (AudioTrack.PLAYSTATE_PLAYING != mTrack.playState) {
                Log.e(TAG, "Start failed. Invalid state: ${mTrack.playState}")
                return FkResult.FK_INVALID_STATE
            }
            strategy?.farTimestampInNano = Long.MIN_VALUE
            mIsPlaying = true
            mHandler?.sendEmptyMessage(1)
            return FkResult.FK_OK
        } catch (e: IllegalStateException) {
            e.printStackTrace()
        }
        return FkResult.FK_FAIL
    }

    fun pause(): FkResult {
        if (AudioTrack.PLAYSTATE_PLAYING != mTrack.playState) {
            Log.e(TAG, "Invalid state: ${mTrack.playState}")
            return FkResult.FK_INVALID_STATE
        }
        try {
            mIsPlaying = false
            mTrack.pause()
            return FkResult.FK_OK
        } catch (e: IllegalStateException) {
            e.printStackTrace()
        }
        return FkResult.FK_FAIL
    }

    fun stop(): FkResult {
        if (AudioTrack.PLAYSTATE_PLAYING != mTrack.playState && AudioTrack.PLAYSTATE_PAUSED != mTrack.playState) {
            Log.e(TAG, "Invalid state: ${mTrack.playState}")
            return FkResult.FK_INVALID_STATE
        }
        try {
            mIsPlaying = false
            mTrack.stop()
            return FkResult.FK_OK
        } catch (e: IllegalStateException) {
            e.printStackTrace()
        }
        return FkResult.FK_FAIL
    }

    fun release(): FkResult {
        stop()
        if (AudioTrack.PLAYSTATE_STOPPED != mTrack.playState) {
            Log.e(TAG, "Invalid state: ${mTrack.playState}")
            return FkResult.FK_INVALID_STATE
        }
        mThread.quitSafely()
        mTrack.release()
        try {
            bis?.close()
            bis = null
        } catch (e: java.lang.Exception) {
            e.printStackTrace()
        }
        return FkResult.FK_OK
    }
}