package com.alimin.pic.widget

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.graphics.Point
import android.util.AttributeSet
import android.view.View

class PointView : View {
    private val paint = Paint(Paint.ANTI_ALIAS_FLAG)
    private val linePaint = Paint(Paint.ANTI_ALIAS_FLAG)
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
        linePaint.color = Color.BLACK
        linePaint.strokeWidth = 1f
    }

    override fun onDraw(canvas: Canvas?) {
        super.onDraw(canvas)
        canvas?.drawLine(
            0.0f, measuredHeight / 2.0f,
            measuredWidth.toFloat(), measuredHeight / 2.0f, linePaint
        )
        canvas?.drawLine(
            measuredWidth / 2.0f, 0.0f,
            measuredWidth / 2.0f, measuredHeight.toFloat(), linePaint
        )
        canvas?.drawPoint(point.x.toFloat(), point.y.toFloat(), paint)
    }

    fun showPoint(point: Point) {
        this.point.set(point.x, point.y)
        invalidate()
    }
}