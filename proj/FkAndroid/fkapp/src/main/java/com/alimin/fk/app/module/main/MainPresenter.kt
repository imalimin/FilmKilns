package com.alimin.fk.app.module.main

import androidx.fragment.app.FragmentActivity
import com.alimin.fk.app.model.GalleryModel
import com.alimin.fk.app.model.impl.GalleryModelImpl

class MainPresenter(private val view: MainContract.View,
                    private val model: GalleryModel = GalleryModelImpl()) : MainContract.Presenter() {
    init {
        view.presenter = this
    }

    override fun <T : FragmentActivity> queryAll(owner: T) {
        model.queryAll(owner) { data, error ->
            if (!error.success()) {
                view.showError(error.error, error.msg)
            } else {
                view.onShowAll(data)
            }
        }
    }

    override fun start() {
    }
}