package com.alimin.pic.module.image

import android.graphics.Rect
import android.util.Size
import android.view.Surface
import com.alimin.fk.core.FkPaint
import com.alimin.fk.engine.FkGetLayersListener
import com.alimin.fk.entity.FkRational
import com.alimin.fk.pb.FkImageLayerOuterClass
import com.lmy.mvp.BaseView
import com.lmy.mvp.ClosablePresenter

interface OnLayerUpdateListener {
    fun onLayers(layers: List<FkImageLayerOuterClass.FkImageLayer>)
    fun onLayer(layer: FkImageLayerOuterClass.FkImageLayer)
}

class ImageContract {
    interface View : BaseView<Presenter> {
        fun onImageSaved(file: String)
        fun onImageSaving()
    }

    abstract class Presenter : ClosablePresenter() {
        abstract fun create()
        abstract fun destroy()
        abstract fun notifyRender()
        abstract fun selectLayer(layerId: Int)
        abstract fun isSelectedLayer(layerId: Int) : Boolean
        abstract fun newLayerWithFile(path: String)
        abstract fun newLayerWithColor(size: Size, red: Int, green: Int, blue: Int, alpha: Int)
        abstract fun removeLayer(layerId: Int)
        abstract fun detachFromSurface(surface: Surface)
        abstract fun attachToSurface(surface: Surface)
        abstract fun postTranslate(dx: Int, dy: Int)
        abstract fun postRotate(dr: FkRational)
        abstract fun postScale(ds: FkRational)
        abstract fun getLayers(listener: FkGetLayersListener): Int
        abstract fun crop(rect: Rect)
        abstract fun save(file: String)
        abstract fun save()
        abstract fun load()
        abstract fun addLayerUpdateListener(l: OnLayerUpdateListener)
        abstract fun removeLayerUpdateListener(l: OnLayerUpdateListener)
        abstract fun drawPath(x: Int, y: Int)
        abstract fun drawPathFinish()
        abstract fun getPaint(): FkPaint
    }
}