package com.alimin.pic.module.main

import androidx.fragment.app.FragmentActivity
import com.alimin.pic.model.GalleryItem
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