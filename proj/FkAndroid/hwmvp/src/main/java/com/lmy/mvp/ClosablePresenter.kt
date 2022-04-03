package com.lmy.mvp

import com.lmy.mvp.model.BaseModel


abstract class ClosablePresenter : BasePresenter {

    /**
     * 防止内存泄漏
     */
    override fun stop() {
        val fields = this::class.java.declaredFields
        for (f in fields) {
            f.isAccessible = true
            try {
                val obj = f.get(this)
                if (obj == null || obj !is BaseModel) continue
//                if (null == obj.queue) continue
//                loge("${this::class.java.simpleName}:${obj::class.java.simpleName} stop")
//                obj.queue.cancelAll(obj)
            } catch (e: IllegalAccessException) {
                e.printStackTrace()
            }

        }
    }
}