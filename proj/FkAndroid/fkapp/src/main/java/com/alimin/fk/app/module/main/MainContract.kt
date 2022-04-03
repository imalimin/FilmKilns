package com.alimin.fk.app.module.main

import com.lmy.mvp.BaseView
import com.lmy.mvp.ClosablePresenter

interface MainContract {
    interface View : BaseView<Presenter> {
//        fun onShowVideoList(items: List<Video>)
    }

    abstract class Presenter : ClosablePresenter() {
//        abstract fun queryHotVideoList(page: Int)
    }
}