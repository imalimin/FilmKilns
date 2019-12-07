package com.lmy.hwvcnative.processor

import android.os.Handler
import android.os.Looper
import android.view.Surface
import com.lmy.hwvcnative.CPPObject
import com.lmy.hwvcnative.entity.AlRational
import com.lmy.hwvcnative.entity.AlResult

class AlImageProcessor private constructor() : CPPObject() {
    private val mMainHandler = Handler(Looper.getMainLooper())
    private var onSaveListener: OnSaveListener? = null

    init {
        handler = create()
    }

    fun release() {
        if (!isNativeNull()) {
            release(handler)
        }
        handler = 0L
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
    fun setScale(id: Int, scale: AlRational): Int {
        if (!isNativeNull()) {
            return setScale(handler, id, scale.num, scale.den)
        }
        return AlResult.FAILED
    }

    /**
     * 累乘缩放图层
     * @param id 图层id
     * @param ds 缩放因子
     * @return   {@link AlResult}
     */
    fun postScale(id: Int, ds: AlRational): Int {
        if (!isNativeNull()) {
            return postScale(handler, id, ds.num, ds.den)
        }
        return AlResult.FAILED
    }

    /**
     * 旋转图层
     * @param id 图层id
     * @param r  旋转弧度，分数表示，没有乘以PI，顺时针方向为正向(CW)
     * @return   {@link AlResult}
     */
    fun setRotation(id: Int, r: AlRational): Int {
        if (!isNativeNull()) {
            return setRotation(handler, id, r.num, r.den)
        }
        return AlResult.FAILED
    }

    /**
     * 累加旋转图层
     * @param id 图层id
     * @param dr 旋转弧度，分数表示，没有乘以PI，顺时针方向为正向(CW)
     * @return   {@link AlResult}
     */
    fun postRotation(id: Int, dr: AlRational): Int {
        if (!isNativeNull()) {
            return postRotation(handler, id, dr.num, dr.den)
        }
        return AlResult.FAILED
    }

    /**
     * 移动图层
     * @param id 图层id
     * @param x  相对于画布的x轴位移，画布中心为0点，x∈[-1, 1]为画布可见范围，绝对值大于1时会超出画布范围而不可见
     * @param y  相对于画布的y轴位移，同x
     * @return   {@link AlResult}
     */
    fun setTranslate(id: Int, x: Float, y: Float): Int {
        if (!isNativeNull()) {
            return setTranslate(handler, id, x, y)
        }
        return AlResult.FAILED
    }

    /**
     * 移动图层，会累加前一次的位移
     * @param id 图层id
     * @param dx  相对于画布的x轴位移，画布中心为0点
     * @param dy  相对于画布的y轴位移，同dx
     * @return   {@link AlResult}
     */
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

    /**
     * 根据View坐标获取layer id
     * 用于点击选择图层
     * @param x 相对于View的x轴坐标
     * @param y 相对于View的y轴坐标
     * @return  图层id，负数表示没找到
     */
    fun getLayer(x: Float, y: Float): Int {
        if (!isNativeNull()) {
            return getLayer(handler, x, y)
        }
        return AlResult.FAILED
    }

    /**
     * 对图层进行裁剪
     * @param id      图层id
     * @param left   左, left∈[0, 1]
     * @param top    上, top∈[0, 1]
     * @param right  右, right∈[0, 1]
     * @param bottom 下, bottom∈[0, 1]
     * @return       {@link AlResult}
     */
    fun cropLayer(id: Int, left: Float, top: Float, right: Float, bottom: Float): Int {
        if (!isNativeNull()) {
            return cropLayer(handler, id, left, top, right, bottom)
        }
        return AlResult.FAILED
    }

    /**
     * 保存图片
     * @return {@link AlResult}
     */
    fun save(path: String): Int {
        if (!isNativeNull()) {
            return save(handler, path)
        }
        return AlResult.FAILED
    }

    private external fun create(): Long
    private external fun release(handler: Long)
    private external fun updateWindow(handler: Long, surface: Surface)
    private external fun invalidate(handler: Long)
    private external fun setCanvas(handler: Long, w: Int, h: Int, color: Int)
    private external fun addLayer(handler: Long, path: String): Int
    private external fun moveLayerIndex(handler: Long, id: Int, index: Int): Int
    private external fun removeLayer(handler: Long, id: Int): Int
    private external fun setScale(handler: Long, id: Int, scaleNum: Int, scaleDen: Int): Int
    private external fun postScale(handler: Long, id: Int, dsNum: Int, dsDen: Int): Int
    private external fun setRotation(handler: Long, id: Int, rNum: Int, rDen: Int): Int
    private external fun postRotation(handler: Long, id: Int, drNum: Int, drDen: Int): Int
    private external fun setTranslate(handler: Long, id: Int, x: Float, y: Float): Int
    private external fun postTranslate(handler: Long, id: Int, dx: Float, dy: Float): Int
    private external fun setAlpha(handler: Long, id: Int, alpha: Float): Int
    private external fun getLayer(handler: Long, x: Float, y: Float): Int
    private external fun cropLayer(handler: Long, id: Int, left: Float, top: Float,
                                   right: Float, bottom: Float): Int

    private external fun save(handler: Long, path: String): Int


    /***************************/
    /**      Listener         **/
    /***************************/
    interface OnSaveListener {
        fun onSave(code: Int, msg: String?, path: String?)
    }

    fun setOnSaveListener(listener: OnSaveListener) {
        this.onSaveListener = listener
    }

    fun setOnSaveListener(listener: (code: Int, msg: String?, path: String?) -> Unit) {
        setOnSaveListener(object : OnSaveListener {
            override fun onSave(code: Int, msg: String?, path: String?) {
                listener(code, msg, path)
            }
        })
    }

    /***************************/
    /** Callback from native  **/
    /***************************/

    /**
     * 图像保存完成回调
     * @param code {@link AlResult}
     * @param msg  状态信息
     * @param path 保存路径
     */
    fun onSave(code: Int, msg: String?, path: String?) {
        mMainHandler.post {
            onSaveListener?.onSave(code, msg, path)
        }
    }


    companion object {
        fun create(): AlImageProcessor = AlImageProcessor()
    }
}