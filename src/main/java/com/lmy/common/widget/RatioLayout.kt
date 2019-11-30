package com.lmy.common.widget

import android.content.Context
import android.support.annotation.AttrRes
import android.util.AttributeSet
import android.view.View
import android.widget.FrameLayout
import com.lmy.common.R

class RatioLayout : FrameLayout {
    private var mRatioY: Float = 1f

    constructor(context: Context) : super(context) {
        resolveAttribute(context, null, 0, 0)
    }

    constructor(context: Context, attrs: AttributeSet?)
            : super(context, attrs) {
        resolveAttribute(context, attrs, 0, 0)
    }

    constructor(context: Context, attrs: AttributeSet?, @AttrRes defStyleAttr: Int)
            : super(context, attrs, defStyleAttr) {
        resolveAttribute(context, attrs, defStyleAttr, 0)
    }

    private fun resolveAttribute(context: Context, attrs: AttributeSet?, defStyleAttr: Int, defStyleRes: Int) {
        val a = context.theme.obtainStyledAttributes(attrs, R.styleable.app, defStyleAttr, defStyleRes)
        var i = 0
        val count = a.indexCount
        while (i < count) {
            val attr = a.getIndex(i)
            if (attr == R.styleable.app_ratioY)
                mRatioY = a.getFloat(attr, 1f)
            i++
        }
        if (mRatioY == 0f) mRatioY = 1f
        a.recycle()
    }

    override fun onMeasure(widthMeasureSpec: Int, heightMeasureSpec: Int) {
        if (mRatioY > 0) { // 宽度已知，根据比例，设置高度
            val width = View.MeasureSpec.getSize(widthMeasureSpec)
            super.onMeasure(View.MeasureSpec.makeMeasureSpec(width,
                    View.MeasureSpec.EXACTLY), View.MeasureSpec.makeMeasureSpec(
                    (width * mRatioY).toInt(), View.MeasureSpec.EXACTLY))
        } else { // 系统默认测量
            super.onMeasure(widthMeasureSpec, heightMeasureSpec)
        }
    }
}