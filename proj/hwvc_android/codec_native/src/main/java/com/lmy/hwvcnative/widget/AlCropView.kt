package com.lmy.hwvcnative.widget

import android.content.Context
import android.graphics.PointF
import android.util.AttributeSet
import android.view.View

class AlCropView : View {
    private val lt = PointF()
    private val lb = PointF()
    private val rb = PointF()
    private val rt = PointF()

    constructor(context: Context) : super(context) {
        initialize()
    }

    constructor(context: Context, attrs: AttributeSet?)
            : super(context, attrs) {
        initialize()
    }

    constructor(context: Context, attrs: AttributeSet?, defStyleAttr: Int)
            : super(context, attrs, defStyleAttr) {
        initialize()
    }

    constructor(context: Context, attrs: AttributeSet, defStyleAttr: Int, defStyleRes: Int)
            : super(context, attrs, defStyleAttr, defStyleRes) {
        initialize()
    }

    private fun initialize() {

    }
}