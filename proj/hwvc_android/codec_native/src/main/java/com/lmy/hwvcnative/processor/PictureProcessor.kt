package com.lmy.hwvcnative.processor

import android.view.Surface
import androidx.annotation.Keep
import com.lmy.hwvcnative.CPPObject
import com.lmy.hwvcnative.FilterSupport
import com.lmy.hwvcnative.filter.Filter

@Keep
class PictureProcessor : CPPObject(), FilterSupport {
    private var filter: Filter? = null

    init {
        handler = create()
    }

    fun prepare(surface: Surface) {
        if (0L == handler) return
        prepare(handler, surface)
    }

    fun updateWindow(surface: Surface) {
        if (0L == handler) return
        updateWindow(handler, surface)
    }

    fun show(file: String) {
        if (0L == handler) return
        show(handler, file)
    }

    fun release() {
        release(handler)
        handler = 0
    }

    override fun setFilter(filter: Filter) {
        if (0L == handler) return
        this.filter = filter
        setFilter(handler, filter.handler)
    }

    override fun getFilter(): Filter? {
        return this.filter
    }

    override fun invalidate() {
        if (0L == handler) return
        invalidate(handler)
    }

    private external fun create(): Long
    private external fun prepare(handler: Long, surface: Surface)
    private external fun updateWindow(handler: Long, surface: Surface)
    private external fun show(handler: Long, file: String)
    private external fun setFilter(handler: Long, filter: Long)
    private external fun invalidate(handler: Long)
    private external fun release(handler: Long)
}