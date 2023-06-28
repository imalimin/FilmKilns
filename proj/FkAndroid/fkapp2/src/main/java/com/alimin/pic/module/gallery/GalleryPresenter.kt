package com.alimin.pic.module.gallery

import androidx.fragment.app.FragmentActivity
import com.alimin.pic.model.GalleryModel
import com.alimin.pic.model.impl.GalleryModelImpl

class GalleryPresenter(
    private val view: GalleryContract.View,
    private val model: GalleryModel = GalleryModelImpl()
) : GalleryContract.Presenter() {
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