package com.alimin.fk.app.module.main

import androidx.fragment.app.FragmentActivity
import com.alimin.fk.app.model.GalleryItem
import com.lmy.mvp.BaseView
import com.lmy.mvp.ClosablePresenter

interface MainContract {
    interface View : BaseView<Presenter> {
        fun onShowAll(items: List<GalleryItem>)
    }

    abstract class Presenter : ClosablePresenter() {
        abstract fun <T> queryAll(owner: T) where T : FragmentActivity
    }
}