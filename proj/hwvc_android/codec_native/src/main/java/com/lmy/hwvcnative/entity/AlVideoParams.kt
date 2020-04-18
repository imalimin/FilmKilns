package com.lmy.hwvcnative.entity

class AlVideoParams(
    var width: Int,
    var height: Int,
    var bitLevel: Int = 3,
    var profile: String = "Main",
    var preset: String = "superfast",
    var enableHardware: Boolean = true
) {
}