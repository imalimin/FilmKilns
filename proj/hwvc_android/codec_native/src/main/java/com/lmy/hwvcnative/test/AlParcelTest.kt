package com.lmy.hwvcnative.test

import com.lmy.hwvcnative.entity.AlParcel
import com.lmy.hwvcnative.entity.AlParcelable

object AlParcelTest {
    external fun testParcel(): ByteArray
}

class AlTestModel(parcel: AlParcel) : AlParcelable(parcel) {
    private var int0 = 0
    private var flt0 = 0f
    private var int1 = 0
    private var d0 = 0.0

    init {
        int0 = parcel.readInt()
        flt0 = parcel.readFloat()
        int1 = parcel.readInt()
        d0 = parcel.readDouble()
    }

    override fun toString(): String {
        return "{ int0=$int0, flt0=$flt0, int1=$int1, d0=$d0 }"
    }
}