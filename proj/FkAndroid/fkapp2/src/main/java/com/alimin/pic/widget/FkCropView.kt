package com.alimin.pic.widget

import android.content.Context
import android.graphics.*
import android.util.AttributeSet
import android.view.MotionEvent
import android.view.View
import kotlin.math.abs

class FkCropView : View {
    enum class Loc { LT, LB, RB, RT, C }

    private val lt = PointF()
    private val rb = PointF()
    //Just for draw
    private val lb = PointF()
    private val rt = PointF()
    private var loc: Loc? = null
    private val lastTouchPointF = PointF()
    private val hRectF = RectF()
    private val vRectF = RectF()
    private val paint = Paint(Paint.ANTI_ALIAS_FLAG)
    private val linePaint = Paint(Paint.ANTI_ALIAS_FLAG)
    private val gridPaint = Paint(Paint.ANTI_ALIAS_FLAG)
    private var strokeWidth: Float = 2f
    private var onChangeListener: OnChangeListener? = null

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
        paint.color = Color.parseColor("#88CCCCCC")
        paint.strokeWidth = strokeWidth
        linePaint.color = Color.parseColor("#55000000")
        linePaint.strokeWidth = strokeWidth
        gridPaint.color = Color.parseColor("#22000000")
        gridPaint.strokeWidth = strokeWidth
        val width = 9f * 6
        val height = 9f
        hRectF.set(0f, 0f, width, height)
        vRectF.set(0f, 0f, height, width)
    }

    override fun onMeasure(widthMeasureSpec: Int, heightMeasureSpec: Int) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec)
        val width = View.MeasureSpec.getSize(widthMeasureSpec)
        val height = View.MeasureSpec.getSize(heightMeasureSpec)
        lt.x = width / 4f
        lt.y = height / 3f
        rb.x = width * 3 / 4f
        rb.y = height * 2 / 3f
    }

    override fun onTouchEvent(event: MotionEvent): Boolean {
        when (event.action) {
            MotionEvent.ACTION_CANCEL, MotionEvent.ACTION_UP -> {
                loc = null
                return true
            }
            MotionEvent.ACTION_DOWN -> {
                lastTouchPointF.set(event.x, event.y)
                val radius = hRectF.width()
                val srcRect = RectF(-radius, -radius, radius, radius)
                var rect = RectF(srcRect)
                rect.offset(lt.x, lt.y)
                if (rect.contains(lastTouchPointF.x, lastTouchPointF.y)) {
                    loc = Loc.LT
                    parent.requestDisallowInterceptTouchEvent(true)
                    return true
                }
                rect = RectF(srcRect)
                rect.offset(lt.x, rb.y)
                if (rect.contains(lastTouchPointF.x, lastTouchPointF.y)) {
                    loc = Loc.LB
                    parent.requestDisallowInterceptTouchEvent(true)
                    return true
                }
                rect = RectF(srcRect)
                rect.offset(rb.x, rb.y)
                if (rect.contains(lastTouchPointF.x, lastTouchPointF.y)) {
                    loc = Loc.RB
                    parent.requestDisallowInterceptTouchEvent(true)
                    return true
                }
                rect = RectF(srcRect)
                rect.offset(rb.x, lt.y)
                if (rect.contains(lastTouchPointF.x, lastTouchPointF.y)) {
                    loc = Loc.RT
                    parent.requestDisallowInterceptTouchEvent(true)
                    return true
                }
                rect = RectF(lt.x, lt.y, rb.x, rb.y)
                if (rect.contains(lastTouchPointF.x, lastTouchPointF.y)) {
                    loc = Loc.C
                    parent.requestDisallowInterceptTouchEvent(true)
                    return true
                }
            }
            MotionEvent.ACTION_MOVE -> {
                if (null != loc) {
                    val dx = event.x - lastTouchPointF.x
                    val dy = event.y - lastTouchPointF.y
                    val lt = lt
                    val rb = rb
                    when (loc) {
                        Loc.LT -> {
                            lt.offset(dx, dy)

                            lt.x = Math.min(lt.x, rb.x - MIN_CROP_SIZE)
                            lt.y = Math.min(lt.y, rb.y - MIN_CROP_SIZE)
                        }
                        Loc.LB -> {
                            lt.offset(dx, 0f)
                            rb.offset(0f, dy)

                            lt.x = Math.min(lt.x, rb.x - MIN_CROP_SIZE)
                            rb.y = Math.max(lt.y + MIN_CROP_SIZE, rb.y)
                        }
                        Loc.RB -> {
                            rb.offset(dx, dy)

                            rb.x = Math.max(lt.x + MIN_CROP_SIZE, rb.x)
                            rb.y = Math.max(lt.y + MIN_CROP_SIZE, rb.y)
                        }
                        Loc.RT -> {
                            rb.offset(dx, 0f)
                            lt.offset(0f, dy)

                            rb.x = Math.max(lt.x + MIN_CROP_SIZE, rb.x)
                            lt.y = Math.min(lt.y, rb.y - MIN_CROP_SIZE)
                        }
                        Loc.C -> {
                            lt.offset(dx, dy)
                            rb.offset(dx, dy)
                        }

                        else -> {}
                    }
                    if (loc == Loc.C) {
                        if (lt.x < 0 || rb.x > measuredWidth) {
                            lt.offset(-dx, 0f)
                            rb.offset(-dx, 0f)
                        }
                        if (lt.y < 0 || rb.y > measuredHeight) {
                            lt.offset(0f, -dy)
                            rb.offset(0f, -dy)
                        }
                    } else {
                        checkBound()
                    }
                    lastTouchPointF.set(event.x, event.y)
                    onChangeListener?.onChange(this)
                    postInvalidate()
                    return true
                }
            }
        }
        return super.onTouchEvent(event)
    }

    private fun checkBound() {
        lt.x = Math.max(0f, lt.x)
        lt.y = Math.max(0f, lt.y)

        rb.x = Math.min(measuredWidth.toFloat(), rb.x)
        rb.y = Math.min(measuredHeight.toFloat(), rb.y)
    }

    override fun onDraw(canvas: Canvas) {
        super.onDraw(canvas)
        lb.set(lt.x, rb.y)
        rt.set(rb.x, lt.y)
        for (i in 1 until 3) {
            val y = lt.y + i * abs(lt.y - rb.y) / 3
            canvas.drawLine(lt.x, y, rb.x, y, gridPaint)
        }
        for (i in 1 until 3) {
            val x = lt.x + i * abs(lt.x - rb.x) / 3
            canvas.drawLine(x, lt.y, x, rb.y, gridPaint)
        }
        drawLine(canvas, lt, lb, linePaint)
        drawLine(canvas, lb, rb, linePaint)
        drawLine(canvas, rb, rt, linePaint)
        drawLine(canvas, rt, lt, linePaint)

        //水平
        canvas.drawRect(
            lt.x + hRectF.left - vRectF.width(), lt.y + hRectF.top - hRectF.height(),
            lt.x + hRectF.right - vRectF.width(), lt.y + hRectF.bottom - hRectF.height(), paint
        )
        //垂直
        canvas.drawRect(
            lt.x + vRectF.left - vRectF.width(), lt.y + vRectF.top - hRectF.height(),
            lt.x + vRectF.right - vRectF.width(), lt.y + vRectF.bottom - hRectF.height(), paint
        )

        //水平
        canvas.drawRect(
            lb.x + hRectF.left - vRectF.width(), lb.y + hRectF.top,
            lb.x + hRectF.right - vRectF.width(), lb.y + hRectF.bottom, paint
        )
        //垂直
        canvas.drawRect(
            lb.x + vRectF.left - vRectF.width(), lb.y - vRectF.height() + hRectF.height(),
            lb.x + vRectF.right - vRectF.width(), lb.y + hRectF.height(), paint
        )

        //水平
        canvas.drawRect(
            rb.x - hRectF.width() + vRectF.width(), rb.y,
            rb.x + vRectF.width(), rb.y + hRectF.height(), paint
        )
        //垂直
        canvas.drawRect(
            rb.x, rb.y - vRectF.height() + hRectF.height(),
            rb.x + vRectF.width(), rb.y + hRectF.height(), paint
        )

        //水平
        canvas.drawRect(
            rt.x + hRectF.left - hRectF.width() + vRectF.width(),
            rt.y + hRectF.top - hRectF.height(),
            rt.x + hRectF.right - hRectF.width() + vRectF.width(),
            rt.y + hRectF.bottom - hRectF.height(),
            paint
        )
        //垂直
        canvas.drawRect(
            rt.x + vRectF.left, rt.y + vRectF.top - hRectF.height(),
            rt.x + vRectF.right, rt.y + vRectF.bottom - hRectF.height(), paint
        )
    }

    private fun drawLine(canvas: Canvas?, start: PointF, end: PointF, paint: Paint) {
        canvas?.drawLine(start.x, start.y, end.x, end.y, paint)
    }

    fun getCropRectF(): RectF = RectF(
        lt.x,
        lt.y,
        rb.x,
        rb.y
    )

    fun getCropRect(): Rect = Rect(
        lt.x.toInt(),
        lt.y.toInt(),
        rb.x.toInt(),
        rb.y.toInt()
    )

    fun reset() {
        lt.x = measuredWidth / 4f
        lt.y = measuredHeight / 4f
        rb.x = measuredWidth * 3 / 4f
        rb.y = measuredHeight * 3 / 4f
    }

    fun setOnChangeListener(listener: OnChangeListener) {
        this.onChangeListener = listener
    }

    fun setOnChangeListener(listener: (view: View) -> Unit) {
        setOnChangeListener(object : OnChangeListener {
            override fun onChange(view: View) {
                listener(view)
            }
        })
    }


    interface OnChangeListener {
        fun onChange(view: View)
    }

    companion object {
        const val MIN_CROP_SIZE = 64
    }
}