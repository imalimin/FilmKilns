package com.alimin.fk.app.model.entity

class Status<T> {
    var status: Int = 0
    var msg: String = "Unknown"
    var data: T? = null
    val success: Boolean
        get() = SUCCESS == status
    val error: Error?
        get() = if (!success) {
            Error(status, msg)
        } else {
            null
        }

    constructor()

    companion object {
        val SUCCESS = 200
    }
}