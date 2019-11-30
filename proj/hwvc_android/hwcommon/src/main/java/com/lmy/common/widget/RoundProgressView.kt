package com.lmy.common.widget

import android.content.Context
import android.graphics.*
import android.support.annotation.AttrRes
import android.util.AttributeSet
import android.view.View

class RoundProgressView : View {
    private val paint = Paint(Paint.ANTI_ALIAS_FLAG)
    private var progress: Float = 0f
    private var strokeWidth: Float = 8f
    private val point = PointF()
    private val rect = RectF()

    constructor(context: Context) : this(context, null)

    constructor(context: Context, attrs: AttributeSet?)
            : this(context, attrs, 0)

    constructor(context: Context, attrs: AttributeSet?, @AttrRes defStyleAttr: Int)
            : this(context, attrs, defStyleAttr, 0)

    constructor(context: Context, attrs: AttributeSet?, @AttrRes defStyleAttr: Int, defStyleRes: Int)
            : super(context, attrs, defStyleAttr) {
        resolveAttribute(context, attrs, defStyleAttr, defStyleRes)
    }

    fun setProgress(progress: Float) {
        this.progress = progress
        invalidate()
    }

    private fun resolveAttribute(context: Context, attrs: AttributeSet?, defStyleAttr: Int, defStyleRes: Int) {
        paint.strokeWidth = strokeWidth
        paint.color = Color.BLACK
    }

    override fun onDraw(canvas: Canvas) {
        val radius = (Math.min(measuredWidth, measuredHeight)
                - Math.max(paddingLeft + paddingRight, paddingTop + paddingBottom)) / 2f - strokeWidth
        point.x = measuredWidth / 2f + paddingLeft - paddingRight
        point.y = measuredHeight / 2f + paddingTop - paddingBottom
        rect.set(point.x - radius, point.y - radius, point.x + radius, point.y + radius)
        paint.style = Paint.Style.STROKE
        canvas.drawCircle(point.x, point.y, radius, paint)
        paint.style = Paint.Style.FILL
        canvas.drawArc(rect, -90f, 360 * progress, true, paint)
    }
}