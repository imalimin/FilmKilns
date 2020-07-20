package com.lmy.hwvcnative.tools

object AlFFUtils {
    external fun exec(cmd: String): Int

    /**
     * 0x1: Video
     * 0x2: Audio
     */
    external fun trackInfo(file: String): Int
}