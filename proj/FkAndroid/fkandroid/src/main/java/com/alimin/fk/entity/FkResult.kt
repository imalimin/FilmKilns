package com.alimin.fk.entity

class FkResult private constructor(private val value: Int) {
    companion object {
        val FK_OK = FkResult(0)
        val FK_FAIL = FkResult(-1)
        val FK_INVALID_STATE = FkResult(-2)
        val FK_PROTOCOL_NOT_ACCEPT = FkResult(-3)
        val FK_INVALID_DATA = FkResult(-4)
        val FK_EMPTY_DATA = FkResult(-5)
        val FK_SKIP = FkResult(-6)
        val FK_NPE = FkResult(-7)
        val FK_FILE_NOT_FOUND = FkResult(-8)
    }

    operator fun compareTo(other: FkResult): Int {
        return value.compareTo(other.value)
    }
}