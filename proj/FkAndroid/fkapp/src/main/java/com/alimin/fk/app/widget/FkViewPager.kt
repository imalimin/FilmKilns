package com.alimin.fk.app.widget

import android.content.Context
import android.util.AttributeSet
import android.view.MotionEvent
import androidx.viewpager.widget.ViewPager

class FkViewPager : ViewPager {

    constructor(context: Context) : super(context) {
        initialize()
    }

    constructor(context: Context, attrs: AttributeSet?)
            : super(context, attrs) {
        initialize()
    }

    private fun initialize() {

    }

    override fun dispatchTouchEvent(ev: MotionEvent?): Boolean {
        return false
    }
}