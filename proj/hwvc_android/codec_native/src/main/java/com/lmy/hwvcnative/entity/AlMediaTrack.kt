package com.lmy.hwvcnative.entity

data class AlMediaTrack(
    var id: Int,
    var type: Int
) : Comparable<AlMediaTrack> {

    override fun compareTo(other: AlMediaTrack): Int = this.id.compareTo(other.id)
}

object AlMediaType {
    const val TYPE_VIDEO = 0
    const val TYPE_AUDIO = 1
    const val TYPE_VIDEO_REF = 2
    const val TYPE_AUDIO_REF = 3
}