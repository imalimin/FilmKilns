package com.lmy.hwvcnative.entity

class AlMediaClip : AlParcelable, Comparable<AlMediaClip> {
    var id: Int = -1
    var seqIn = 0L
    var trimIn = 0L
    var duration = 0L
    var path = ""

    constructor(parcel: AlParcel) : super(parcel) {
        id = parcel.readInt()
        seqIn = parcel.readLong()
        trimIn = parcel.readLong()
        duration = parcel.readLong()
        path = parcel.readString()
    }

    constructor(id: Int) : super(AlParcel.from(ByteArray(0))) {
        this.id = id
    }

    override fun compareTo(other: AlMediaClip): Int = this.id.compareTo(other.id)

}