package com.alimin.fk.app.model.entity

data class Error(
    var error: Int = 0,
    var msg: String
) {
    companion object {
        fun unknown(): Error = Error(-1, "Unknown error")
        fun success(): Error = Error(0, "success")
    }

    fun success(): Boolean = error == 0
}