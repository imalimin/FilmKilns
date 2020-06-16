package com.lmy.hwvcnative.devices.entity

import android.annotation.SuppressLint

class CamMetadata(
    var id: String,
    var facing: kFacing,
    var focalDistance: Float
)

enum class kFacing {
    FRONT,
    BACK
}