package com.alimin.fk.app.module.image

import android.graphics.Color
import android.graphics.Rect
import android.util.Size
import android.view.Surface
import com.alimin.fk.app.model.ImageEngineModel
import com.alimin.fk.app.model.impl.ImageEngineModelImpl
import com.alimin.fk.core.FkPaint
import com.alimin.fk.define.kScaleType
import com.alimin.fk.engine.FkGetLayersListener
import com.alimin.fk.engine.FkImage
import com.alimin.fk.engine.FkImageModel
import com.alimin.fk.entity.FkRational
import com.alimin.fk.entity.FkResult
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
    private var paint = FkPaint(10, Color.WHITE)
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

    override fun selectLayer(layerId: Int) {
        if (layerId > 0) {
            curLayer = layerId
        }
    }

    override fun isSelectedLayer(layerId: Int) : Boolean = curLayer == layerId

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

    override fun newLayerWithColor(size: Size, red: Int, green: Int, blue: Int, alpha: Int) {
        val layer = engine.newLayerWithColor(size, red, green, blue, alpha)
        if (curLayer < 0) {
            selectLayer(layer)
        }
        engine.notifyRender()
        if (layer > 0) {
            notifyLayers()
        }
    }

    override fun removeLayer(layerId: Int) {
        val ret = engine.removeLayer(layerId)
        if (FkResult.FK_OK == ret) {
            if (curLayer == layerId) {
                curLayer = -1
            }
            engine.notifyRender()
            if (layerId > 0) {
                notifyLayers()
            }
        }
    }

    override fun detachFromSurface(surface: Surface) {
        engine.detachFromSurface(surface)
    }

    override fun attachToSurface(surface: Surface) {
        engine.attachToSurface(surface, kScaleType.CENTER_INSIDE)
    }

    override fun postTranslate(dx: Int, dy: Int) {
        if (checkCurLayer()) {
            engine.postTranslate(curLayer, dx, dy)
        }
    }

    override fun postRotate(dr: FkRational) {
        if (checkCurLayer()) {
            engine.postRotation(curLayer, dr.num, dr.den)
        }
    }

    override fun postScale(ds: FkRational) {
        if (checkCurLayer()) {
            engine.postScale(curLayer, ds.num.toFloat() / ds.den, ds.num.toFloat() / ds.den)
        }
    }

    override fun getLayers(listener: FkGetLayersListener): Int {
        return modelEngine.getLayers(listener)
    }

    override fun crop(rect: Rect) {
        if (checkCurLayer()) {
            engine.crop(rect)
        }
    }

    override fun save(file: String) {
        engine.save(file)
    }

    private fun checkCurLayer(): Boolean {
        return curLayer > 0
    }

    @Synchronized
    override fun addLayerUpdateListener(l: OnLayerUpdateListener) {
        layerUpdateListeners.add(l)
    }

    @Synchronized
    override fun removeLayerUpdateListener(l: OnLayerUpdateListener) {
        layerUpdateListeners.remove(l)
    }

    override fun drawPath(x: Int, y: Int) {
        engine.drawPath(curLayer, x, y, paint)
    }

    override fun drawPathFinish() {
        engine.drawPathFinish(curLayer)
    }

    override fun getPaint(): FkPaint = paint
}