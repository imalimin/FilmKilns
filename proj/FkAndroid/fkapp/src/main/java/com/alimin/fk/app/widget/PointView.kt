package com.alimin.fk.app.widget

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.graphics.Point
import android.util.AttributeSet
import android.view.View

class PointView : View {
    private val paint = Paint(Paint.ANTI_ALIAS_FLAG)
    private val point = Point(0, 0)

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
        paint.color = Color.BLACK
        paint.strokeWidth = 15f
    }

    override fun onDraw(canvas: Canvas?) {
        super.onDraw(canvas)
        canvas?.drawPoint(point.x.toFloat(), point.y.toFloat(), paint)
    }

    fun showPoint(point: Point) {
        this.point.set(point.x, point.y)
        invalidate()
    }
}