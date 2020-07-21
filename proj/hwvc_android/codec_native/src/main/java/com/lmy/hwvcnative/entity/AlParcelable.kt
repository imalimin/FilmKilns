package com.lmy.hwvcnative.entity

import java.nio.ByteBuffer
import java.nio.ByteOrder

class AlParcel private constructor(data: ByteArray) {
    private var buf = ByteBuffer.wrap(data).apply { order(ByteOrder.LITTLE_ENDIAN) }
    fun readByte(): Byte {
        return buf.get()
    }

    fun readInt(): Int {
        return buf.int
    }

    fun readLong(): Long {
        return buf.long
    }

    fun readFloat(): Float {
        return buf.float
    }

    fun readDouble(): Double {
        return buf.double
    }

    companion object {
        fun from(data: ByteArray): AlParcel {
            return AlParcel(data)
        }
    }
}

abstract class AlParcelable(parcel: AlParcel) {
}