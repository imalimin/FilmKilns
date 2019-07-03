package com.lmy.hwvcnative.media

import android.content.Context
import android.content.pm.PackageManager
import android.media.AudioManager
import android.util.Log
import com.lmy.hwvcnative.CPPObject

class HwEchoPlayer(context: Context) : CPPObject() {
    init {
        val lowLatency = context.packageManager.hasSystemFeature(PackageManager.FEATURE_AUDIO_LOW_LATENCY)//45ms
        val audioPro = context.packageManager.hasSystemFeature(PackageManager.FEATURE_AUDIO_PRO)//20ms
        val am = context.getSystemService(Context.AUDIO_SERVICE) as AudioManager
        val sampleRateStr: String? = am.getProperty(AudioManager.PROPERTY_OUTPUT_SAMPLE_RATE)
        val sampleRate: Int = sampleRateStr?.let { str ->
            Integer.parseInt(str).takeUnless { it == 0 }
        } ?: 44100
        val samplesPerBufferStr: String? = am.getProperty(AudioManager.PROPERTY_OUTPUT_FRAMES_PER_BUFFER)
        val samplesPerBuffer: Int = samplesPerBufferStr?.let { str ->
            Integer.parseInt(str).takeUnless { it == 0 }
        } ?: 256
        handler = create(2, sampleRate, 0x0010, samplesPerBuffer)
        Log.i("HWVC", "HwEchoPlayer lowLatency=$lowLatency, audioPro=$audioPro, sampleRate=$sampleRate, samplesPerBuffer=$samplesPerBuffer")
    }

    fun start() {
        if (0L == handler) {
            return
        }
        start(handler)
    }

    fun stop() {
        if (0L == handler) {
            return
        }
        stop(handler)
    }

    fun release() {
        if (0L == handler) {
            return
        }
        release(handler)
        handler = 0L
    }

    private external fun create(channels: Int, sampleHz: Int, format: Int, minBufferSize: Int): Long
    private external fun start(handler: Long)
    private external fun stop(handler: Long)
    private external fun release(handler: Long)
}