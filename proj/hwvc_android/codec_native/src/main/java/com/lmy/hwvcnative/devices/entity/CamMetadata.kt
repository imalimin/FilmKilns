package com.lmy.hwvcnative.devices.entity

data class CamMetadata(
    var id: String,
    var facing: kFacing,
    var focalDistance: Float,
    var lensLength: Float
)

enum class kFacing {
    FRONT,
    BACK
}