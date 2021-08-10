package com.alimin.fk.device

import android.media.AudioFormat
import android.media.AudioRecord
import android.media.MediaRecorder
import com.alimin.fk.entity.FkAudioSettings
import com.alimin.fk.entity.FkResult

open class FkMicrophone : FkDevice() {
    private lateinit var mRecord: AudioRecord
    private var settings: FkAudioSettings? = null

    fun init(settings: FkAudioSettings): FkResult {
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
        val minBufSize = AudioRecord.getMinBufferSize(sampleRate, channelConfig, audioFormat)
        mRecord = AudioRecord(
            MediaRecorder.AudioSource.MIC, sampleRate,
            channelConfig, audioFormat, minBufSize
        )
        return FkResult.FK_OK
    }
}