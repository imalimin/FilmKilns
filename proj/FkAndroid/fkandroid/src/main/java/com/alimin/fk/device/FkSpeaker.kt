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

    private lateinit var mTrack: AudioTrack
    private val timestamp = AudioTimestamp()
    private var settings: FkAudioSettings? = null
    private var mByteBuffer: ByteBuffer? = null
    private var minBufSize = 0
    private val mThread = HandlerThread(TAG)
    private var mHandler: Handler? = null

    private var bis: FileInputStream? = null

    private fun handleMessage(msg: Message) {
        while (AudioTrack.PLAYSTATE_PLAYING == mTrack.playState) {
            try {
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                    val size = bis?.read(mByteBuffer!!.array(), 0, minBufSize)
                    mTrack.write(mByteBuffer!!, size!!, AudioTrack.WRITE_BLOCKING)
                }
            } catch (e: Exception) {
                e.printStackTrace()
            }
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
                mTrack.getTimestamp(timestamp)
            }
            Log.i(TAG, "Read: ${timestamp.nanoTime}/${System.nanoTime()}, ${timestamp.framePosition}")
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
            settings.sampleRate,
            settings.channels,
            AudioFormat.ENCODING_PCM_16BIT,
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
            mHandler?.sendEmptyMessage(0)
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
            mTrack.stop()
            return FkResult.FK_OK
        } catch (e: IllegalStateException) {
            e.printStackTrace()
        }
        return FkResult.FK_FAIL
    }

    fun release(): FkResult {
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