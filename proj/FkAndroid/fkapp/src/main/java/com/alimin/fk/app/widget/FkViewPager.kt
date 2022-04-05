package com.alimin.fk.app.widget

import android.content.Context
import android.util.AttributeSet
import android.util.Log
import android.view.MotionEvent
import android.view.ViewGroup
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

    override fun onTouchEvent(ev: MotionEvent?): Boolean {
        return if (isEnabled) super.onTouchEvent(ev) else false
    }
}