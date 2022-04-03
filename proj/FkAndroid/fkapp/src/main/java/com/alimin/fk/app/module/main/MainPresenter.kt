package com.alimin.fk.app.module.main

import com.alimin.fk.app.model.GalleryModel
import com.alimin.fk.app.model.impl.GalleryModelImpl

class MainPresenter(private val view: MainContract.View,
                    private val model: GalleryModel = GalleryModelImpl()) : MainContract.Presenter() {
    init {
        view.presenter = this
    }

    override fun start() {
    }
}