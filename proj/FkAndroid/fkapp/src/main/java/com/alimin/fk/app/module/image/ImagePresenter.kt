package com.alimin.fk.app.module.image

import android.view.Surface
import com.alimin.fk.app.model.ImageEngineModel
import com.alimin.fk.app.model.impl.ImageEngineModelImpl
import com.alimin.fk.define.kScaleType
import com.alimin.fk.engine.FkGetLayersListener
import com.alimin.fk.engine.FkImage
import com.alimin.fk.engine.FkImageModel
import com.alimin.fk.entity.FkRational
import com.alimin.fk.pb.FkImageLayerOuterClass
import java.io.File

class ImagePresenter(
    private val view: ImageContract.View,
    workspace: String,
    private val model: ImageEngineModel = ImageEngineModelImpl()
) : ImageContract.Presenter() {
    private val cacheFile: File
    private val engine: FkImage
    private val modelEngine: FkImageModel
    private var curLayer = -1
    private var layerUpdateListeners = ArrayList<OnLayerUpdateListener>()

    init {
        view.presenter = this
        cacheFile = File(File(workspace), "/${System.currentTimeMillis()}.fkp")
        engine = FkImage(workspace)
        modelEngine = FkImageModel(engine)
    }

    private fun notifyLayers() {
        getLayers(object : FkGetLayersListener {
            override fun onGetLayers(layers: List<FkImageLayerOuterClass.FkImageLayer>) {
                synchronized(this) {
                    layerUpdateListeners.forEach { it.onLayers(layers) }
                }
            }
        })
    }

    override fun create() {
        engine.create()
        modelEngine.create()
    }

    override fun start() {
        engine.start()
        modelEngine.start()
        modelEngine.load(cacheFile.absolutePath)
    }

    override fun stop() {
        modelEngine.save(cacheFile.absolutePath)
        modelEngine.stop()
        engine.stop()
    }

    override fun destroy() {
        modelEngine.destroy()
        engine.destroy()
        synchronized(this) {
            layerUpdateListeners.clear()
        }
    }

    override fun notifyRender() {
        engine.notifyRender()
    }

    override fun newLayerWithFile(path: String) {
        val layer = engine.newLayerWithFile(path)
        if (curLayer < 0) {
            curLayer = layer
        }
        engine.notifyRender()
        if (layer > 0) {
            notifyLayers()
        }
    }

    override fun detachFromSurface(surface: Surface) {
        engine.detachFromSurface(surface)
    }

    override fun attachToSurface(surface: Surface) {
        engine.attachToSurface(surface, kScaleType.CENTER_INSIDE)
    }

    override fun postTranslate(dx: Int, dy: Int) {
        engine.postTranslate(curLayer, dx, dy)
    }

    override fun postRotate(dr: FkRational) {
        engine.postRotation(curLayer, dr.num, dr.den)
    }

    override fun postScale(ds: FkRational) {
        engine.postScale(curLayer, ds.num.toFloat() / ds.den, ds.num.toFloat() / ds.den)
    }

    override fun getLayers(listener: FkGetLayersListener): Int {
        return modelEngine.getLayers(listener)
    }

    @Synchronized
    override fun addLayerUpdateListener(l: OnLayerUpdateListener) {
        layerUpdateListeners.add(l)
    }

    @Synchronized
    override fun removeLayerUpdateListener(l: OnLayerUpdateListener) {
        layerUpdateListeners.remove(l)
    }
}