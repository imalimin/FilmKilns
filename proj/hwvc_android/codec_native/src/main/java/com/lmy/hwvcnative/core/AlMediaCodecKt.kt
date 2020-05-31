package com.lmy.hwvcnative.core

import android.media.MediaCodec
import android.media.MediaCodecInfo
import android.media.MediaCodecList
import android.media.MediaFormat
import android.os.Build
import android.util.Log
import android.view.Surface
import androidx.annotation.Keep
import com.lmy.hwvcnative.entity.AlResult
import java.nio.ByteBuffer

@Keep
class AlMediaCodecKt(
    private val mime: String
) {
    private var codec: MediaCodec = MediaCodec.createEncoderByType(mime)
    private var outputFormat: MediaFormat? = null
    private var iBuffer: ByteBuffer? = null
    private var oBuffer: ByteBuffer? = null

    /**
     * @return AlResult
     */
    fun configure(
        w: Int,
        h: Int,
        bitrate: Int,
        format: Int,
        iFrameInterval: Int,
        fps: Int,
        flags: Int
    ): Int {
        val f = createVideoFormat(mime, w, h, bitrate, format, iFrameInterval, fps)
            ?: return AlResult.FAILED
        try {
            codec.configure(f, null, null, flags)
        } catch (e: Exception) {
            e.printStackTrace()
            Log.i(TAG, "configure failed.")
            return AlResult.FAILED
        }
        return AlResult.SUCCESS
    }

    fun createInputSurface(): Surface? {
        try {
            return codec.createInputSurface()
        } catch (e: Exception) {
            e.printStackTrace()
            Log.i(TAG, "createSurface failed.")
        }
        return null
    }

    /**
     * @return AlResult
     */
    fun start(): Int {
        try {
            codec.start()
        } catch (e: Exception) {
            e.printStackTrace()
            Log.i(TAG, "start failed.")
            return AlResult.FAILED
        }
        return AlResult.SUCCESS
    }

    fun stop(): Int {
        try {
            codec.stop()
        } catch (e: Exception) {
            e.printStackTrace()
            Log.i(TAG, "stop failed.")
            return AlResult.FAILED
        }
        return AlResult.SUCCESS
    }

    fun flush(): Int {
        try {
            codec.flush()
        } catch (e: Exception) {
            e.printStackTrace()
            Log.i(TAG, "flush failed.")
            return AlResult.FAILED
        }
        return AlResult.SUCCESS
    }

    fun release() {
        codec.release()
    }

    fun dequeueInputBuffer(timeoutUs: Long): Int {
        try {
            return codec.dequeueInputBuffer(timeoutUs)
        } catch (e: Exception) {
            e.printStackTrace()
            Log.i(TAG, "dequeueInputBuffer failed.")
        }
        return -1
    }

    /**
     * @return AlResult
     */
    fun queueInputBuffer(
        index: Int,
        offset: Int,
        size: Int,
        presentationTimeUs: Long,
        flags: Int
    ): Int {
        try {
            codec.queueInputBuffer(index, offset, size, presentationTimeUs, flags)
        } catch (e: Exception) {
            e.printStackTrace()
            Log.i(TAG, "queueInputBuffer failed.")
            return AlResult.FAILED
        }
        return AlResult.SUCCESS
    }

    fun getInputBuffer(index: Int): ByteBuffer? {
        try {
            iBuffer = codec.getInputBuffer(index)
            iBuffer?.rewind()
            return iBuffer
        } catch (e: Exception) {
            e.printStackTrace()
            Log.i(TAG, "getInputBuffer failed.")
        }
        return null
    }

    /**
     * @return {idx, offset, size, presentationTimeUs, flags}
     */
    fun dequeueOutputBuffer(timeoutUs: Long): LongArray {
        try {
            val info = MediaCodec.BufferInfo()
            val idx = codec.dequeueOutputBuffer(info, timeoutUs)
            return longArrayOf(
                idx.toLong(), info.offset.toLong(), info.size.toLong(),
                info.presentationTimeUs, info.flags.toLong()
            )
        } catch (e: Exception) {
            e.printStackTrace()
            Log.i(TAG, "dequeueOutputBuffer failed.")
        }
        return longArrayOf(MediaCodec.INFO_TRY_AGAIN_LATER.toLong(), -1, -1, -1, -1)
    }

    fun getOutputBuffer(index: Int): ByteBuffer? {
        try {
            oBuffer = codec.getOutputBuffer(index)
            oBuffer?.rewind()
            return oBuffer
        } catch (e: Exception) {
            e.printStackTrace()
            Log.i(TAG, "getOutputBuffer failed.")
        }
        return null
    }

    /**
     * @return AlResult
     */
    fun releaseOutputBuffer(index: Int, render: Boolean): Int {
        try {
            codec.releaseOutputBuffer(index, render)
        } catch (e: Exception) {
            e.printStackTrace()
            Log.i(TAG, "releaseOutputBuffer failed.")
            return AlResult.FAILED
        }
        return AlResult.SUCCESS

    }

    fun getOutputFormatBuffer(name: String): ByteBuffer? {
        checkOutputFormat()
        val buf = outputFormat!!.getByteBuffer(name)
        buf?.rewind()
        val tmp = ByteBuffer.allocateDirect(buf.capacity())
        tmp.put(buf)
        tmp?.rewind()
        return tmp
    }

    fun getOutputFormatInteger(name: String): Int {
        checkOutputFormat()
        return outputFormat!!.getInteger(name)
    }

    private fun checkOutputFormat() {
        if (null == outputFormat) {
            outputFormat = codec.outputFormat
        }
    }

    companion object {
        private const val TAG = "AlMediaCodecKt"

        /**
         * MediaCodec兼容性问题：
         * 1. 部分7.0以上机型开启high效果不明显，如LG G6
         * 2. 部分机型开启high会导致BufferInfo.presentationTimeUs乱序，具体表现为0, 100000, 50000, 150000，如小米NOTE PRO
         * @param ignoreDevice 忽略设备兼容性检测
         */
        fun createVideoFormat(
            mime: String,
            w: Int,
            h: Int,
            bitrate: Int,
            format: Int,
            iFrameInterval: Int,
            fps: Int,
            ignoreDevice: Boolean = false
        ): MediaFormat? {
            val codecInfo = getCodecInfo(mime, true)
            if (!ignoreDevice && null == codecInfo) {//Unsupport codec type
                return null
            }
            return MediaFormat().apply {
                setString(MediaFormat.KEY_MIME, mime)
                setInteger(MediaFormat.KEY_WIDTH, w)
                setInteger(MediaFormat.KEY_HEIGHT, h)
                setInteger(MediaFormat.KEY_COLOR_FORMAT, format)
                setInteger(MediaFormat.KEY_BIT_RATE, bitrate)
                setInteger(MediaFormat.KEY_FRAME_RATE, fps)
                setInteger(MediaFormat.KEY_I_FRAME_INTERVAL, iFrameInterval)
//            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
//                mediaFormat.setInteger(MediaFormat.KEY_BITRATE_MODE, context.video.bitrateMode)
//                mediaFormat.setInteger(
//                    MediaFormat.KEY_COMPLEXITY,
//                    MediaCodecInfo.EncoderCapabilities.BITRATE_MODE_CBR
//                )
//                if (!ignoreDevice)
//                    setProfileLevel(context, codecInfo!!, mediaFormat)
//            }
            }
        }

        private fun getCodecInfo(mime: String, isEncode: Boolean): MediaCodecInfo? {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                val codecList = MediaCodecList(MediaCodecList.ALL_CODECS)
                codecList.codecInfos
                    .filter { it.isEncoder == isEncode }
                    .forEach { info ->
                        info.supportedTypes.forEach {
                            if (it.equals(mime, ignoreCase = true)) {
                                return info
                            }
                        }
                    }
            } else {
                val count = MediaCodecList.getCodecCount()
                (0 until count)
                    .map { MediaCodecList.getCodecInfoAt(it) }
                    .filter { it.isEncoder == isEncode }
                    .forEach { info ->
                        info.supportedTypes.forEach {
                            if (it.equals(mime, ignoreCase = true)) {
                                return info
                            }
                        }
                    }
            }
            return null
        }
    }
}