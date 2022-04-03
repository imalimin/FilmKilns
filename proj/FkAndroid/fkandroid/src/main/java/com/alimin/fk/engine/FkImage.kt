package com.alimin.fk.engine

import android.graphics.Point
import android.view.Choreographer
import android.view.Surface

class FkImage(val workspace: String) : FkEngine() {
    private var mSyncLock = Object()
    override fun onCreateInstance(): Long = nativeCreateInstance(workspace)
    override fun create() {
        super.create()
        if (!isNull()) {
            nativeCreate(getHandle())
        }
    }

    override fun destroy() {
        super.destroy()
        synchronized(mSyncLock) {
            if (!isNull()) {
                nativeDestroy(getHandle())
                releaseHandle()
            }
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

//    private var lastTime = 0L
    fun notifyRender(): Int {
        if (isNull()) {
            return -1
        }
        Choreographer.getInstance().postFrameCallback {
            synchronized(mSyncLock) {
                if (!isNull()) {
//                    val time = System.currentTimeMillis()
//                    Log.i("alimin1", "${time - lastTime}")
//                    lastTime = time
                    nativeNotifyRender(getHandle())
                }
            }
        }
        return 0
    }

    fun setCanvasSize(width: Int, height: Int): Int {
        if (!isNull()) {
            return nativeSetCanvasSize(getHandle(), width, height)
        }
        return -1
    }

    /**
     * @param layer Layer ID, ID 0 is Canvas.
     * @param dx Delta x position of view
     * @param dy Delta y position of view
     * @return Result code.
     */
    fun postTranslate(layer: Int, dx: Int, dy: Int): Int {
        if (!isNull()) {
            return nativePostTranslate(getHandle(), layer, dx, dy)
        }
        return -1
    }

    fun postScale(layer: Int, dx: Float, dy: Float): Int {
        if (!isNull()) {
            return nativePostScale(getHandle(), layer, dx, dy)
        }
        return -1
    }

    fun postRotation(layer: Int, num: Int, den: Int): Int {
        if (!isNull()) {
            return nativePostRotation(getHandle(), layer, num, den)
        }
        return -1
    }

    /**
     * @param layer Layer ID, ID 0 is Canvas.
     * @param size  Size of point
     * @param point Point of view
     * @return Result code.
     */
    fun drawPoint(layer: Int, color: Long, size: Int, point: Point): Int {
        if (!isNull()) {
            return nativeDrawPoint(getHandle(), layer, color, size, point.x, point.y)
        }
        return -1
    }

    fun crop(layer: Int, leftTop: Point, rightBottom: Point): Int {
        if (!isNull()) {
            return nativeCrop(getHandle(), layer, leftTop.x, leftTop.y, rightBottom.x, rightBottom.y)
        }
        return -1
    }

    fun cropLayer(layer: Int, leftTop: Point, rightBottom: Point): Int {
        if (!isNull()) {
            return nativeCrop(getHandle(), layer, leftTop.x, leftTop.y, rightBottom.x, rightBottom.y)
        }
        return -1
    }

    private external fun nativeCreateInstance(workspace: String): Long
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
    private external fun nativeSetCanvasSize(handle: Long, width: Int, height: Int): Int

    private external fun nativeNotifyRender(handle: Long): Int
    private external fun nativePostTranslate(handle: Long, layer: Int, dx: Int, dy: Int): Int
    private external fun nativePostScale(handle: Long, layer: Int, dx: Float, dy: Float): Int
    private external fun nativePostRotation(handle: Long, layer: Int, num: Int, den: Int): Int
    private external fun nativeDrawPoint(handle: Long, layer: Int, color: Long, size: Int, x: Int, y: Int): Int
    private external fun nativeCrop(handle: Long, layer: Int, left: Int, top: Int, right: Int, bottom: Int): Int
    private external fun nativeCropLayer(handle: Long, layer: Int, left: Int, top: Int, right: Int, bottom: Int): Int
}