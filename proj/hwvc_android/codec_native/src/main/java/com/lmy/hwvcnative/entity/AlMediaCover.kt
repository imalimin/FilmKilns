package com.lmy.hwvcnative.entity

import java.nio.ByteBuffer

class AlMediaCover : AlParcelable, Comparable<AlMediaCover> {
    var timestamp: Long = 0
    var width: Int = 0
    var height: Int = 0
    var buf: ByteBuffer? = null

    constructor(parcel: AlParcel) : super(parcel) {
        timestamp = parcel.readLong()
        width = parcel.readInt()
        height = parcel.readInt()
        buf = parcel.readByteBuffer()
    }

    constructor(id: Int, type: Int) : super(AlParcel.from(ByteArray(0))) {
    }

    override fun compareTo(other: AlMediaCover): Int = this.timestamp.compareTo(other.timestamp)
}