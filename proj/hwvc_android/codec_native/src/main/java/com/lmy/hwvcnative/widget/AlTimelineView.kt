package com.lmy.hwvcnative.widget

import android.content.Context
import android.content.res.Resources
import android.graphics.*
import android.util.AttributeSet
import android.util.Log
import android.util.TypedValue
import androidx.annotation.AttrRes
import java.text.SimpleDateFormat
import java.util.*
import kotlin.collections.ArrayList

class AlTimelineView : AlAbsView {
    private val paint = Paint(Paint.ANTI_ALIAS_FLAG)
    private val fmt = SimpleDateFormat("mm:ss")
    private var durationInUS = 0L
    private val textSize = Point()
    private val cursorRect = Rect()
    private var cursorSize = 3f
    private var spaceSize = 0f
    private val textVec = ArrayList<String>()

    constructor(context: Context) : super(context) {
        onResolveAttribute(context, null, 0, 0)
        onInitialize(context)
    }

    constructor(context: Context, attrs: AttributeSet?)
            : super(context, attrs) {
        onResolveAttribute(context, attrs, 0, 0)
        onInitialize(context)
    }

    constructor(context: Context, attrs: AttributeSet?, @AttrRes defStyleAttr: Int)
            : super(context, attrs, defStyleAttr) {
        onResolveAttribute(context, attrs, defStyleAttr, 0)
        onInitialize(context)
    }

    private fun onResolveAttribute(
        context: Context,
        attrs: AttributeSet?,
        defStyleAttr: Int,
        defStyleRes: Int
    ) {

    }

    private fun onInitialize(context: Context) {
        paint.strokeWidth = 3f
        paint.color = Color.GRAY
        paint.textSize = applyDimension(TypedValue.COMPLEX_UNIT_SP, 10f)
        val bounds = Rect()
        paint.getTextBounds("00:00", 0, 5, bounds)
        textSize.x = bounds.width()
        textSize.y = bounds.height()
        spaceSize = textSize.x.toFloat()

        cursorSize = applyDimension(TypedValue.COMPLEX_UNIT_DIP, 2f)
        cursorRect.set(0, 0, textSize.x / 2, cursorSize.toInt())
    }

    override fun onMeasure(widthMeasureSpec: Int, heightMeasureSpec: Int) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec)
        val width = MeasureSpec.getSize(widthMeasureSpec)
        setMeasuredDimension(
            width,
            textSize.y + paddingTop + paddingBottom
        )
    }

    private fun applyDimension(unit: Int, value: Float): Float {
        val r = if (context == null) {
            Resources.getSystem()
        } else {
            context.resources
        }
        return TypedValue.applyDimension(unit, value, r.displayMetrics)
    }

    fun setDuration(us: Long) {
        if (us > 0 && this.durationInUS != us) {
            this.durationInUS = us
            post {
                textVec.clear()
                measureText()
                invalidate()
            }
        }
    }

    fun getDuration(): Long = durationInUS

    private fun measureText(): Int {
        if (durationInUS <= 0) {
            textVec.clear()
            return 0
        }
        val maxWidth = measuredWidth + textSize.x - paddingLeft - paddingRight
        if (textVec.isNotEmpty()) {
            val tmp = (maxWidth - textSize.x * textVec.size) / (textVec.size - 1).toFloat()
            if (tmp < textSize.x + cursorRect.width() * 2 && tmp > cursorRect.width()) {
                spaceSize = tmp
                return textVec.size
            }
        }
        val count = (maxWidth / (textSize.x + cursorRect.width())).toInt()
        spaceSize = if (1 == count) {
            (maxWidth - textSize.x).toFloat()
        } else {
            (maxWidth - textSize.x * count) / (count - 1).toFloat()
        }
        if (textVec.size == count) {
            return count
        }
        textVec.clear()
        if (count > 1) {
            for (i in 0 until count) {
                textVec.add(fmt.format(Date(i * durationInUS / (count - 1) / 1000)))
            }
        } else {
            textVec.add(fmt.format(Date(0)))
        }
        return count
    }

    override fun onDraw(canvas: Canvas?) {
        super.onDraw(canvas)
        val count = measureText()
        for (i in 0 until count) {
            val text = textVec[i]
            val x = paddingLeft - textSize.x / 2f + ((textSize.x + spaceSize) * i).toFloat()
            canvas?.drawText(text, x, (measuredHeight + textSize.y) / 2f, paint)
            if (i < count - 1) {
                canvas?.drawCircle(
                    x + textSize.x + spaceSize / 2f,
                    measuredHeight / 2f,
                    cursorSize / 2f, paint
                )
            }
        }
    }

}