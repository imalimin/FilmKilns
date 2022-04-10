package com.alimin.fk.app.module.image

import android.view.Surface
import com.alimin.fk.app.model.ImageEngineModel
import com.alimin.fk.app.model.impl.ImageEngineModelImpl
import com.alimin.fk.define.kScaleType
import com.alimin.fk.engine.FkImage
import com.alimin.fk.engine.FkImageModel
import com.alimin.fk.entity.FkRational
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
    init {
        view.presenter = this
        cacheFile = File(File(workspace), "/${System.currentTimeMillis()}.fkp")
        engine = FkImage(workspace)
        modelEngine = FkImageModel(engine)
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
        modelEngine.getLayers()
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
}