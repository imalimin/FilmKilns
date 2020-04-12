package com.lmy.mvp.proxy.impl

import com.lmy.mvp.ClosablePresenter
import com.lmy.mvp.proxy.ClosablePresenterProxy

class ClosablePresenterProxyImpl : ClosablePresenterProxy {
    override fun stop(thiz: Any) {
        val fields = thiz::class.java.declaredFields
        for (f in fields) {
            f.isAccessible = true
            try {
                val obj = f.get(thiz)
                if (obj == null || obj !is ClosablePresenter) continue
                obj.stop()
            } catch (e: IllegalAccessException) {
                e.printStackTrace()
            }

        }
    }

    companion object {
        fun proxy(thiz: Any) {
            ClosablePresenterProxyImpl().stop(thiz)
        }
    }
}