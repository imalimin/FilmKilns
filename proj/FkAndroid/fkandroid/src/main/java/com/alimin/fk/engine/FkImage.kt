package com.alimin.fk.engine

import android.view.Surface

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

    fun attachToSurface(surface: Surface?): Int {
        if (!isNull()) {
            return nativeSetSurface(getHandle(), surface)
        }
        return -1
    }

    fun detachFromSurface(surface: Surface?) {

    }

    fun newLayerWithFile(path: String): Int {
        if (!isNull()) {
            return nativeNewLayerWithFile(getHandle(), path)
        }
        return -1
    }

    fun newLayerWithColor(
        width: Int,
        height: Int,
        red: Int,
        green: Int,
        blue: Int,
        alpha: Int
    ): Int {
        if (!isNull()) {
            return nativeNewLayerWithColor(getHandle(), width, height, red, green, blue, alpha)
        }
        return -1
    }

    fun notifyRender(): Int {
        if (!isNull()) {
            return nativeNotifyRender(getHandle())
        }
        return -1
    }

    private external fun nativeCreateInstance(): Long
    private external fun nativeCreate(handle: Long)
    private external fun nativeDestroy(handle: Long)
    private external fun nativeStart(handle: Long)
    private external fun nativeStop(handle: Long)
    private external fun nativeSetSurface(handle: Long, surface: Surface?): Int
    private external fun nativeNewLayerWithFile(handle: Long, path: String): Int
    private external fun nativeNewLayerWithColor(
        handle: Long,
        width: Int,
        height: Int,
        red: Int,
        green: Int,
        blue: Int,
        alpha: Int
    ): Int
    private external fun nativeNotifyRender(handle: Long): Int
}