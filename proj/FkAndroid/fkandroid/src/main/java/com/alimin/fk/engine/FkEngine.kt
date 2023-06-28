package com.alimin.fk.engine

abstract class FkEngine {
    private var handle: Long = 0
    abstract fun onCreateInstance(): Long
    open fun create() {
        handle = onCreateInstance()
    }

    open fun destroy() {}
    open fun start() {}
    open fun stop() {}

    protected fun isNull(): Boolean {
        return 0L == handle
    }

    open fun getHandle(): Long {
        return handle
    }

    protected fun releaseHandle() {
        handle = 0L
    }
}