package com.alimin.fk.device

import android.media.AudioFormat
import android.media.AudioRecord
import android.media.MediaRecorder
import android.os.Build
import android.os.Handler
import android.os.HandlerThread
import android.os.Message
import android.util.Log
import com.alimin.fk.entity.FkAudioSettings
import com.alimin.fk.entity.FkResult
import java.io.File
import java.io.FileOutputStream
import java.lang.Exception
import java.nio.ByteBuffer

open class FkMicrophone : FkDevice() {
    companion object {
        const val TAG = "FkMicrophone"
    }

    private lateinit var mRecord: AudioRecord
    private var settings: FkAudioSettings? = null
    private var mByteBuffer: ByteBuffer? = null
    private var minBufSize = 0
    private val mThread = HandlerThread(TAG)
    private var mHandler: Handler? = null

    private var bos: FileOutputStream? = null

    private fun handleMessage(msg: Message) {
        while (AudioRecord.RECORDSTATE_RECORDING == mRecord.recordingState) {
            val size = if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                mRecord.read(mByteBuffer!!, minBufSize, AudioRecord.READ_BLOCKING)
            } else {
                0
            }
            if (size > 0) {
                bos?.write(mByteBuffer!!.array(), 0, size)
            }
            Log.i(TAG, "Read: $size")
        }
        Log.i(TAG, "Stop record loop")
    }

    fun init(settings: FkAudioSettings, path: String): FkResult {
        Log.e(TAG, "Path: $path")
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
        minBufSize = AudioRecord.getMinBufferSize(sampleRate, channelConfig, audioFormat)
        mByteBuffer = ByteBuffer.allocateDirect(minBufSize)
        mRecord = AudioRecord(
            MediaRecorder.AudioSource.MIC, sampleRate,
            channelConfig, audioFormat, minBufSize
        )

        val file = File(path)
        if (file.exists()) {
            file.delete()
        }
        file.createNewFile()
        try {
            bos = FileOutputStream(file)
        } catch (e: Exception) {
            e.printStackTrace()
        }

        mThread.start()
        mHandler = object : Handler(mThread.looper) {
            override fun handleMessage(msg: Message) {
                this@FkMicrophone.handleMessage(msg)
            }
        }
        return FkResult.FK_OK
    }

    fun start(): FkResult {
        if (AudioRecord.STATE_INITIALIZED != mRecord.recordingState) {
            Log.e(TAG, "Invalid state: ${mRecord.recordingState}")
            return FkResult.FK_INVALID_STATE
        }
        try {
            mRecord.startRecording()
            if (AudioRecord.RECORDSTATE_RECORDING != mRecord.recordingState) {
                Log.e(TAG, "Start failed. Invalid state: ${mRecord.recordingState}")
                return FkResult.FK_INVALID_STATE
            }
            mHandler?.sendEmptyMessage(0)
            return FkResult.FK_OK
        } catch (e: IllegalStateException) {
            e.printStackTrace()
        }
        return FkResult.FK_FAIL
    }

    fun stop(): FkResult {
        if (AudioRecord.RECORDSTATE_RECORDING != mRecord.recordingState) {
            Log.e(TAG, "Invalid state: ${mRecord.recordingState}")
            return FkResult.FK_INVALID_STATE
        }
        try {
            mRecord.stop()
            return FkResult.FK_OK
        } catch (e: IllegalStateException) {
            e.printStackTrace()
        }
        return FkResult.FK_FAIL
    }

    fun release(): FkResult {
        if (AudioRecord.RECORDSTATE_STOPPED != mRecord.recordingState) {
            Log.e(TAG, "Invalid state: ${mRecord.recordingState}")
            return FkResult.FK_INVALID_STATE
        }
        mThread.quitSafely()
        mRecord.release()
        try {
            bos?.flush()
            bos?.close()
            bos = null
        } catch (e: Exception) {
            e.printStackTrace()
        }
        return FkResult.FK_OK
    }
}