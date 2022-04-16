package com.alimin.fk.app.module.image

import android.util.Size
import android.view.Surface
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
        abstract fun addLayerUpdateListener(l: OnLayerUpdateListener)
        abstract fun removeLayerUpdateListener(l: OnLayerUpdateListener)
    }
}