package com.alimin.fk.engine

class FkImage() : FkEngine() {
    override fun onCreateInstance(): Long = nativeCreateInstance()
    override fun create() {
        super.create()
        if (!isNull()) {
            nativeCreate(getHandle())
        }
    }

    override fun destroy() {
        super.destroy()
        if (!isNull()) {
            nativeDestroy(getHandle())
            releaseHandle()
        }
    }

    override fun start() {
        super.start()
        if (!isNull()) {
            nativeStart(getHandle())
        }
    }

    override fun stop() {
        super.stop()
        if (!isNull()) {
            nativeStop(getHandle())
        }
    }

    fun newLayer(path: String): Int {
        if (!isNull()) {
            return nativeNewLayer(getHandle(), path)
        }
        return -1
    }

    private external fun nativeCreateInstance(): Long
    private external fun nativeCreate(handle: Long)
    private external fun nativeDestroy(handle: Long)
    private external fun nativeStart(handle: Long)
    private external fun nativeStop(handle: Long)
    private external fun nativeNewLayer(handle: Long, path: String): Int
}