/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/
package com.lmy.hwvcnative.processor

import android.os.Handler
import android.os.Looper
import android.view.Surface
import androidx.annotation.Keep
import com.lmy.hwvcnative.CPPObject
import com.lmy.hwvcnative.FilterSupport
import com.lmy.hwvcnative.filter.Filter

@Keep
class VideoProcessor : CPPObject(), FilterSupport {
    private var filter: Filter? = null
    private var onPlayProgressListener: ((Long, Long) -> Unit)? = null
    private val mHandler = Handler(Looper.getMainLooper())

    init {
        handler = create()
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
    }

    fun setSource(path: String) {
        if (0L == handler) return
        setSource(handler, path)
    }

    fun updateWindow(surface: Surface) {
        if (0L == handler) return
        updateWindow(handler, surface)
    }

    fun start() {
        if (0L == handler) return
        start(handler)
    }

    fun pause() {
        if (0L == handler) return
        pause(handler)
    }

    fun seek(us: Long) {
        if (0L == handler) return
        seek(handler, us)
    }

    fun release() {
        release(handler)
        handler = 0
    }

    fun setOnPlayProgressListener(listener: (Long, Long) -> Unit) {
        this.onPlayProgressListener = listener
    }

    /**
     * Call from jni.
     * @param us Timestamp for play progress.
     */
    fun onPlayProgress(us: Long, duration: Long) {
        mHandler.post {
            onPlayProgressListener?.invoke(us, duration)
        }
    }

    private external fun create(): Long
    private external fun setSource(handler: Long, path: String)
    private external fun updateWindow(handler: Long, surface: Surface)
    private external fun setFilter(handler: Long, filter: Long)
    private external fun start(handler: Long)
    private external fun pause(handler: Long)
    private external fun seek(handler: Long, us: Long)
    private external fun release(handler: Long)
}