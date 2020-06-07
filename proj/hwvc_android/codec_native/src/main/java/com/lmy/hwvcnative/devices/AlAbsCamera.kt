package com.lmy.hwvcnative.devices

import android.graphics.Point
import android.util.Log
import android.util.Size

interface AlAbsCamera {
    enum class CameraIndex { BACK, FRONT }

    fun getCameraSize(): Point
    fun switchCamera(index: CameraIndex)
    fun release()
    fun draw()
    fun getMatrix(): FloatArray
    /**
     * @return nanoseconds
     */
    fun timestamp(): Long

    companion object {
        fun chooseBestSize(sizes: Array<Size>, reqWidth: Int, reqHeight: Int): Size {
            val size = Point(0, 0)
            sizes.forEach {
//                Log.i("AlCamera2", "${it.width}x${it.height}, ${reqWidth}x${reqHeight}")
                if (it.width >= reqHeight//预览宽大于输出宽
                    && it.height >= reqWidth//预览高大于输出高
                    && (it.width * it.height < size.x * size.y || 0 == size.x * size.y)
                ) {//选择像素最少的分辨率
                    size.x = it.height
                    size.y = it.width
                }
            }
            if (0 == size.x || 0 == size.y) {
                throw RuntimeException("Camera invalid size(${size.x} x ${size.y})")
            }
            return Size(size.x, size.y)
        }
    }
}