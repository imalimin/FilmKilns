package com.lmy.common.model.entity

data class Error(var error: Int = -1,
                 var msg: String? = null) {
    companion object {
        val UNKNOWN = Error(-1, "Unknown error")
        fun error(msg: String): Error = Error(-2, msg)
        fun error(code: Int, msg: String): Error = Error(code, msg)
    }
}