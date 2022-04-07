package com.alimin.fk.app.module.image

import android.view.Surface
import com.alimin.fk.entity.FkRational
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
        abstract fun detachFromSurface(surface: Surface)
        abstract fun attachToSurface(surface: Surface)
        abstract fun postTranslate(dx: Int, dy: Int)
        abstract fun postRotate(dr: FkRational)
        abstract fun postScale(ds: FkRational)
    }
}