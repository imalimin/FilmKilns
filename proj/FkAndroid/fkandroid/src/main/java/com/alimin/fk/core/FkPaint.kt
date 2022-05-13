package com.alimin.fk.core

import com.alimin.fk.pb.FkPaintInfoOuterClass

data class FkPaint(
    var strokeWidth: Int = 0,
    var color: Int = 0
) {

    fun toByteArray(): ByteArray {
        return FkPaintInfoOuterClass.FkPaintInfo.newBuilder()
            .setStrokeWidth(strokeWidth)
            .setColor(color)
            .build()
            .toByteArray()
    }
}