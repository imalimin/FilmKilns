package com.alimin.fk.entity

class FkResult private constructor(private val value: Int) {
    companion object {
        val FK_OK = FkResult(0)
        val FK_FAIL = FkResult(-1)
        val FK_INVALID_STATE = FkResult(-2)
    }

    operator fun compareTo(other: FkResult): Int {
        return value.compareTo(other.value)
    }
}