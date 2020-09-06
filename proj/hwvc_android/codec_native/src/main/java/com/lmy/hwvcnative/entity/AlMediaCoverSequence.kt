package com.lmy.hwvcnative.entity

class AlMediaCoverSequence : AlParcelable {
    val covers = ArrayList<AlMediaCover>()

    constructor(parcel: AlParcel) : super(parcel) {
        val size = parcel.readInt()
        for (i in 0 until size) {
            val cover = AlMediaCover(parcel)
            covers.add(cover)
        }
    }

    constructor() : super(AlParcel.from(ByteArray(0))) {
    }
}