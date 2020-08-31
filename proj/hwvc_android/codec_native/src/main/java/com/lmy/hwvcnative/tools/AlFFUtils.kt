package com.lmy.hwvcnative.tools

object AlFFUtils {
    external fun exec(cmd: String): Int

    /**
     * 0x1: Video
     * 0x2: Audio
     */
    external fun trackInfo(file: String): Int

    external fun parseWaveform(
        seqIn: Long,
        duInUS: Long,
        files: Array<String?>,
        seqIns: LongArray,
        trimIns: LongArray,
        dus: LongArray,
        width: Int
    ): FloatArray?

    external fun parseVideoCover(
        seqIn: Long,
        duInUS: Long,
        files: Array<String?>,
        seqIns: LongArray,
        trimIns: LongArray,
        dus: LongArray,
        width: Int
    ): ByteArray?
}