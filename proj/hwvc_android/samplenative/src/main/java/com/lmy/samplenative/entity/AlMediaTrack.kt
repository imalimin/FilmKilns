package com.lmy.samplenative.entity

data class AlMediaTrack(
    var id: Int,
    var type: Int
)

object AlMediaType {
    const val TYPE_VIDEO = 0
    const val TYPE_AUDIO = 1
    const val TYPE_VIDEO_REF = 2
    const val TYPE_AUDIO_REF = 3
}