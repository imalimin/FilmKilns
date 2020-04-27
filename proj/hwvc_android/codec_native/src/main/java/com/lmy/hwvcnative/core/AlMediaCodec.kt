package com.lmy.hwvcnative.core

import android.media.MediaCodec
import android.media.MediaCodecInfo
import android.media.MediaCodecInfo.CodecCapabilities.createFromProfileLevel
import android.media.MediaCodecList
import android.media.MediaFormat
import android.os.Build
import android.util.Log
import java.nio.ByteBuffer

open class AlMediaCodec(
    private val codecId: Int,
    private val encodeMode: Boolean,
    private val makeNalSelf: Boolean
) {
    private var codec: MediaCodec? = null
    private var buffers = arrayOfNulls<ByteBuffer?>(4)
    private lateinit var packet: ByteArray
    private val bufInfo = MediaCodec.BufferInfo()

    fun configure(
        w: Int,
        h: Int,
        bitrate: Int,
        format: Int,
        iFrameInterval: Int,
        fps: Int
    ): Boolean {
        if (encodeMode && !makeNalSelf) {
            val codec = AlMediaCodec(codecId, true, true)
            if (codec.configure(w, h, bitrate, format, iFrameInterval, fps)) {
                buffers[0] = codec.getExtraBuffer("csd-0")
                buffers[1] = codec.getExtraBuffer("csd-1")
                buffers[2] = codec.getExtraBuffer("csd-2")
                buffers[3] = codec.getExtraBuffer("csd-3")
            }
            codec.release()
        }

        try {
            val f = createVideoFormat(codecId, w, h, bitrate, format, iFrameInterval, fps)
                ?: return false
            codec = MediaCodec.createEncoderByType(f.getString(MediaFormat.KEY_MIME))
            codec?.configure(f, null, null, MediaCodec.CONFIGURE_FLAG_ENCODE)
            codec?.start()
        } catch (e: Exception) {
            e.printStackTrace()
            return false
        }
        if (null == codec) {
            return false
        }
        packet = ByteArray(w * h * 3 / 2)
        return true
    }

    fun getExtraBuffer(key: String): ByteBuffer? {
        when (key) {
            "csd-0" -> return buffers[0]
            "csd-1" -> return buffers[1]
            "csd-2" -> return buffers[2]
            "csd-3" -> return buffers[3]
        }
        return null
    }

    fun flush() {
        codec?.flush()
    }

    fun release() {
        flush()
        codec?.stop()
        codec?.release()
    }

    fun start(): Boolean {
        return true
    }

    fun process(data: ByteArray?, pts: Long): Int {
        var ret = push(data, pts)
        if (0 != ret) {
            Log.e(TAG, "HwAndroidCodec::process push failed.")
        }
        ret = pop(2000)
        if (0 != ret) {
//            if (encodeMode) {
//                *pkt = hwPacket;
//            } else {
//                *frame = outFrame;
//            }
        } else if (-541478725 == ret) {
            return ret
        }
        return ret
    }

    fun type(): Int {
        return 0
    }

    private fun push(data: ByteArray?, pts: Long): Int {
        if (null == codec) {
            return -1
        }
        val bufIdx = codec!!.dequeueInputBuffer(2000)
        if (bufIdx < 0) {
            return -1
        }
        if (null == data) {
            codec!!.queueInputBuffer(
                bufIdx, 0, 0, 0,
                MediaCodec.BUFFER_FLAG_END_OF_STREAM
            )
            return 0
        }
        val buf = codec!!.getInputBuffer(bufIdx)
        if (null != buf && buf.capacity() >= data.size) {
            buf.put(data)
        }
        codec!!.queueInputBuffer(bufIdx, 0, data.size, pts, 0)
        return 0
    }

    private fun pop(waitInUS: Long): Int {
        val info = MediaCodec.BufferInfo()
        val bufIdx = codec!!.dequeueOutputBuffer(info, waitInUS)
        when (bufIdx) {
            MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED -> {
                Log.e(TAG, "INFO_OUTPUT_BUFFERS_CHANGED")
                return -1
            }
            MediaCodec.INFO_OUTPUT_FORMAT_CHANGED -> {
                Log.i(TAG, "INFO_OUTPUT_FORMAT_CHANGED")
                val format = codec!!.outputFormat
                if (encodeMode) {
                    val buf0 = format.getByteBuffer("csd-0")
                    val buf1 = format.getByteBuffer("csd-1")
                    if (null != buf0) {
                        val data = ByteArray(buf0.capacity())
                        buffers[0] = ByteBuffer.allocateDirect(buf0.capacity())
                        buffers[0]?.put(data)
                    }
                    if (null != buf1) {
                        val data = ByteArray(buf1.capacity())
                        buffers[1] = ByteBuffer.allocateDirect(buf1.capacity())
                        buffers[1]?.put(data)
                    }
                }
                return -1
            }
            MediaCodec.INFO_TRY_AGAIN_LATER -> {
                Log.i(TAG, "HwAndroidCodec INFO_TRY_AGAIN_LATER")
                return -1
            }
            MediaCodec.BUFFER_FLAG_END_OF_STREAM -> {
                Log.i(TAG, "HwAndroidCodec BUFFER_FLAG_END_OF_STREAM")
                return -1
            }
        }
        if (bufIdx < 0) {
            return -1
        }
        val buf = codec!!.getOutputBuffer(bufIdx)
        if (null == buf) {
            codec!!.releaseOutputBuffer(bufIdx, false)
            return -1
        }
        if (info.flags and MediaCodec.BUFFER_FLAG_END_OF_STREAM > 0) {
            codec!!.releaseOutputBuffer(bufIdx, false)
            return -541478725
        }
        var wrote = false
        if (info.size > 0) {
            if (info.flags and MediaCodec.BUFFER_FLAG_END_OF_STREAM > 0) {
                wrote = false
            } else {
                if (encodeMode) {
                    memcpy(packet, info.size)
                    buf.rewind()
                    buf.get(packet, 4, info.size)
                    bufInfo.size = info.size
                    bufInfo.flags = info.flags
                    bufInfo.presentationTimeUs = info.presentationTimeUs
                }
                wrote = true
            }
        }
        //缓冲区使用完后必须把它还给MediaCodec，以便再次使用，至此一个流程结束，再次循环
        codec!!.releaseOutputBuffer(bufIdx, false)
        return if (wrote) 0 else -1
    }

    fun getBuffer(): ByteArray {
        return packet
    }

    fun getBufferInfo(): LongArray {
        return longArrayOf(
            bufInfo.size + 4L,
            bufInfo.presentationTimeUs,
            if (bufInfo.flags and MediaCodec.BUFFER_FLAG_KEY_FRAME > 0) 0 else -1
        )
    }

    private fun memcpy(dst: ByteArray, src: Int) {
        for (i in 0 until 4) {
            packet[i] = ((src shr 8 * i) and 0xFF).toByte()
        }
    }

    private fun memcpy(dst: ByteArray, src: Long) {
        for (i in 0 until 8) {
            packet[i] = ((src shr 8 * i) and 0xFF).toByte()
        }
    }

    companion object {
        private const val TAG = "AlMediaCodec"

        fun getMime(codecId: Int): String {
            return MediaFormat.MIMETYPE_VIDEO_AVC
        }

        /**
         * MediaCodec兼容性问题：
         * 1. 部分7.0以上机型开启high效果不明显，如LG G6
         * 2. 部分机型开启high会导致BufferInfo.presentationTimeUs乱序，具体表现为0, 100000, 50000, 150000，如小米NOTE PRO
         * @param ignoreDevice 忽略设备兼容性检测
         */
        fun createVideoFormat(
            codecId: Int,
            w: Int,
            h: Int,
            bitrate: Int,
            format: Int,
            iFrameInterval: Int,
            fps: Int,
            ignoreDevice: Boolean = false
        ): MediaFormat? {
            val mime = getMime(codecId)
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

//        private fun setProfileLevel(
//            context: CodecContext,
//            codecInfo: MediaCodecInfo,
//            format: MediaFormat
//        ) {
//            //低于LOLLIPOP的系统不支持profile和level
//            if (Build.VERSION.SDK_INT < Build.VERSION_CODES.LOLLIPOP) return
//            val mime = format.getString(MediaFormat.KEY_MIME)
//            val profileLevels = codecInfo.getCapabilitiesForType(mime).profileLevels
//
//            var selected = MediaCodecInfo.CodecProfileLevel()
//            selected.profile = -1
//            selected.level = -1
//            //查找支持的最佳配置
//            for (p in profileLevels) {
//                if (supportsProfileLevel(p.profile, p.level, profileLevels, mime)) {
//                    if (MediaCodecInfo.CodecProfileLevel.AVCProfileHigh == p.profile
//                        && p.profile > selected.profile
//                        && Build.VERSION.SDK_INT >= Build.VERSION_CODES.N
//                    ) {//AVCProfileHigh需要Android 7.0或以上才支持
//                        selected = p
//                    } else if (MediaCodecInfo.CodecProfileLevel.AVCProfileMain == p.profile
//                        && p.profile > selected.profile
//                    ) {
//                        selected = p
//                    }
//                }
//            }
//            //如果找不到则默认
//            if (-1 == selected.profile) return
//            setProfileLevel(context, format, selected.profile, selected.level)
//        }

//        private fun setProfileLevel(
//            context: CodecContext,
//            format: MediaFormat,
//            profile: Int,
//            level: Int
//        ) {
//            context.video.profile = profile
//            context.video.level = level
//            //level must be set even below M
//            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
//                format.setInteger(MediaFormat.KEY_PROFILE, context.video.profile)
//                format.setInteger(MediaFormat.KEY_LEVEL, context.video.level)
//            }
//        }

        private fun supportsProfileLevel(
            profile: Int, level: Int?,
            profileLevels: Array<MediaCodecInfo.CodecProfileLevel>, mime: String
        ): Boolean {
            for (pl in profileLevels) {
                if (pl.profile != profile) {
                    continue
                }

                // AAC does not use levels
                if (level == null || mime.equals(
                        MediaFormat.MIMETYPE_AUDIO_AAC,
                        ignoreCase = true
                    )
                ) {
                    return true
                }

                // H.263 levels are not completely ordered:
                // Level45 support only implies Level10 support
                if (mime.equals(MediaFormat.MIMETYPE_VIDEO_H263, ignoreCase = true)) {
                    if (pl.level != level && pl.level == MediaCodecInfo.CodecProfileLevel.H263Level45
                        && level > MediaCodecInfo.CodecProfileLevel.H263Level10
                    ) {
                        continue
                    }
                }

                // MPEG4 levels are not completely ordered:
                // Level1 support only implies Level0 (and not Level0b) support
                if (mime.equals(MediaFormat.MIMETYPE_VIDEO_MPEG4, ignoreCase = true)) {
                    if (pl.level != level && pl.level == MediaCodecInfo.CodecProfileLevel.MPEG4Level1
                        && level > MediaCodecInfo.CodecProfileLevel.MPEG4Level0
                    ) {
                        continue
                    }
                }

                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                    val HEVCHighTierLevels =
                        (MediaCodecInfo.CodecProfileLevel.HEVCHighTierLevel1
                                or MediaCodecInfo.CodecProfileLevel.HEVCHighTierLevel2
                                or MediaCodecInfo.CodecProfileLevel.HEVCHighTierLevel21
                                or MediaCodecInfo.CodecProfileLevel.HEVCHighTierLevel3
                                or MediaCodecInfo.CodecProfileLevel.HEVCHighTierLevel31
                                or MediaCodecInfo.CodecProfileLevel.HEVCHighTierLevel4
                                or MediaCodecInfo.CodecProfileLevel.HEVCHighTierLevel41
                                or MediaCodecInfo.CodecProfileLevel.HEVCHighTierLevel5
                                or MediaCodecInfo.CodecProfileLevel.HEVCHighTierLevel51
                                or MediaCodecInfo.CodecProfileLevel.HEVCHighTierLevel52
                                or MediaCodecInfo.CodecProfileLevel.HEVCHighTierLevel6
                                or MediaCodecInfo.CodecProfileLevel.HEVCHighTierLevel61
                                or MediaCodecInfo.CodecProfileLevel.HEVCHighTierLevel62)
                    // HEVC levels incorporate both tiers and levels. Verify tier support.
                    if (mime.equals(MediaFormat.MIMETYPE_VIDEO_HEVC, ignoreCase = true)) {
                        val supportsHighTier = pl.level and HEVCHighTierLevels != 0
                        val checkingHighTier = level and HEVCHighTierLevels != 0
                        // high tier levels are only supported by other high tier levels
                        if (checkingHighTier && !supportsHighTier) {
                            continue
                        }
                    }

                    if (pl.level >= level) {
                        // if we recognize the listed profile/level, we must also recognize the
                        // profile/level arguments.
                        return if (createFromProfileLevel(mime, profile, pl.level) != null) {
                            createFromProfileLevel(mime, profile, level) != null
                        } else true
                    }
                }
            }
            return false
        }
    }
}