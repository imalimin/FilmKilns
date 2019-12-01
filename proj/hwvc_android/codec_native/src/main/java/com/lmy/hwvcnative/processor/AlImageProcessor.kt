package com.lmy.hwvcnative.processor

import android.view.Surface
import com.lmy.hwvcnative.CPPObject
import com.lmy.hwvcnative.entity.AlResult

class AlImageProcessor private constructor() : CPPObject() {
    init {
        handler = create()
    }

    fun release() {
        if (!isNativeNull()) {
            release(handler)
        }
        handler = 0L
    }

    fun prepare(surface: Surface) {
        if (!isNativeNull()) {
            prepare(handler, surface)
        }
    }

    fun updateWindow(surface: Surface) {
        if (!isNativeNull()) {
            updateWindow(handler, surface)
        }
        invalidate()
    }

    fun invalidate() {
        if (!isNativeNull()) {
            invalidate(handler)
        }
    }

    /**
     * 设置画布大小
     * @param w     新的画布宽度
     * @param h     新的画布高度
     * @param color 画布颜色
     */
    fun setCanvas(w: Int, h: Int, color: Int = 0) {
        if (!isNativeNull()) {
            setCanvas(handler, w, h, color)
        }
    }

    /**
     * 添加新的图层
     * @param path 图片文件路径
     * @return     新图层的id，id < 0表示添加失败，可用于后续的图层操作
     */
    fun addLayer(path: String): Int {
        if (!isNativeNull()) {
            return addLayer(handler, path)
        }
        return AlResult.FAILED
    }

    /**
     * 改变图层层级
     * @param id    图层id
     * @param index 图层层级，index >= 0，if(index > layer size) index = layer size
     *              0为最底层，会被所有图层遮挡
     * @return      {@link AlResult}
     */
    fun moveLayerIndex(id: Int, index: Int): Int {
        if (!isNativeNull()) {
            return moveLayerIndex(handler, id, index)
        }
        return AlResult.FAILED
    }

    /**
     * 移除图层
     * @param id 图层id
     * @return   {@link AlResult}
     */
    fun removeLayer(id: Int): Int {
        if (!isNativeNull()) {
            return removeLayer(handler, id)
        }
        return AlResult.FAILED
    }

    /**
     * 缩放图层
     * @param id    图层id
     * @param scale 缩放倍数
     * @return      {@link AlResult}
     */
    fun setScale(id: Int, scale: Float): Int {
        if (!isNativeNull()) {
            return setScale(handler, id, scale)
        }
        return AlResult.FAILED
    }

    /**
     * 旋转图层
     * @param id       图层id
     * @param rotation 旋转角度，单位弧度
     * @return         {@link AlResult}
     */
    fun setRotation(id: Int, rotation: Float): Int {
        if (!isNativeNull()) {
            return setRotation(handler, id, rotation)
        }
        return AlResult.FAILED
    }

    /**
     * 移动图层
     * @param id 图层id
     * @param x  相对于画布的x轴位移，画布中心为0点，范围[-1, 1]，绝对值大于1时会超出画布范围而不可见
     * @param y  相对于画布的y轴位移，同x
     * @return   {@link AlResult}
     */
    fun setTranslate(id: Int, x: Float, y: Float): Int {
        if (!isNativeNull()) {
            return setTranslate(handler, id, x, y)
        }
        return AlResult.FAILED
    }

    fun postTranslate(id: Int, dx: Float, dy: Float): Int {
        if (!isNativeNull()) {
            return postTranslate(handler, id, dx, dy)
        }
        return AlResult.FAILED
    }

    /**
     * 设置图层透明度
     * @param id    图层id
     * @param alpha 图层透明度，范围[0, 1]，1为完全不透明
     * @return      {@link AlResult}
     */
    fun setAlpha(id: Int, alpha: Float): Int {
        if (!isNativeNull()) {
            return setAlpha(handler, id, alpha)
        }
        return AlResult.FAILED
    }

    private external fun create(): Long
    private external fun release(handler: Long)
    private external fun prepare(handler: Long, surface: Surface)
    private external fun updateWindow(handler: Long, surface: Surface)
    private external fun invalidate(handler: Long)
    private external fun setCanvas(handler: Long, w: Int, h: Int, color: Int)
    private external fun addLayer(handler: Long, path: String): Int
    private external fun moveLayerIndex(handler: Long, id: Int, index: Int): Int
    private external fun removeLayer(handler: Long, id: Int): Int
    private external fun setScale(handler: Long, id: Int, scale: Float): Int
    private external fun setRotation(handler: Long, id: Int, rotation: Float): Int
    private external fun setTranslate(handler: Long, id: Int, x: Float, y: Float): Int
    private external fun postTranslate(handler: Long, id: Int, dx: Float, dy: Float): Int
    private external fun setAlpha(handler: Long, id: Int, alpha: Float): Int

    companion object {
        fun create(): AlImageProcessor = AlImageProcessor()
    }
}