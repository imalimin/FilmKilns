package com.alimin.fk.app.module.image

import com.lmy.mvp.BaseView
import com.lmy.mvp.ClosablePresenter

class ImageContract {
    interface View : BaseView<Presenter> {
    }

    abstract class Presenter : ClosablePresenter() {
        abstract fun create()
        abstract fun destroy()
        abstract fun notifyRender()
        abstract fun newLayerWithFile(path: String)
    }
}