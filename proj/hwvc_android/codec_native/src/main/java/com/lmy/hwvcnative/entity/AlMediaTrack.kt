package com.lmy.hwvcnative.entity

class AlMediaTrack : AlParcelable, Comparable<AlMediaTrack> {
    var id: Int = -1
    var type: Int = AlMediaType.TYPE_UNKNOWN
    var seqIn = 0L
    var seqOut = 0L
    var duration = 0L

    constructor(parcel: AlParcel) : super(parcel) {
        id = parcel.readInt()
        type = parcel.readInt()
        seqIn = parcel.readLong()
        seqOut = parcel.readLong()
        duration = parcel.readLong()
    }

    constructor(id: Int, type: Int) : super(AlParcel.from(ByteArray(0))) {
        this.id = id
        this.type = type
    }

    override fun compareTo(other: AlMediaTrack): Int = this.id.compareTo(other.id)
}

object AlMediaType {
    const val TYPE_UNKNOWN = -1
    const val TYPE_VIDEO = 0
    const val TYPE_AUDIO = 1
    const val TYPE_VIDEO_REF = 2
    const val TYPE_AUDIO_REF = 3
}