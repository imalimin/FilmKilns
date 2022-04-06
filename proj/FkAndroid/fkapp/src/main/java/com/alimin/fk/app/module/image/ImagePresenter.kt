package com.alimin.fk.app.module.image

import com.alimin.fk.app.model.ImageEngineModel
import com.alimin.fk.app.model.impl.ImageEngineModelImpl
import com.alimin.fk.engine.FkImage
import com.alimin.fk.engine.FkImageFile
import java.io.File

class ImagePresenter(
    private val view: ImageContract.View,
    workspace: String,
    private val model: ImageEngineModel = ImageEngineModelImpl()
) : ImageContract.Presenter() {
    private val cacheFile: File
    private val engine: FkImage
    private val fileEngine: FkImageFile
    init {
        view.presenter = this
        cacheFile = File(File(workspace), "/${System.currentTimeMillis()}.fkp")
        engine = FkImage(workspace)
        fileEngine = FkImageFile(engine)
    }

    override fun create() {
        engine.create()
        fileEngine.create()
    }

    override fun start() {
        engine.start()
        fileEngine.start()
        fileEngine.load(cacheFile.absolutePath)
    }

    override fun stop() {
        fileEngine.save(cacheFile.absolutePath)
        fileEngine.stop()
        engine.stop()
    }

    override fun destroy() {
        fileEngine.destroy()
        engine.destroy()
    }

    override fun notifyRender() {
        engine.notifyRender()
    }

    override fun newLayerWithFile(path: String) {
        val layer = engine.newLayerWithFile(path)
        engine.notifyRender()
    }
}