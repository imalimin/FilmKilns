package com.lmy.hwvcnative.widget

import android.content.Context
import android.graphics.*
import android.util.AttributeSet
import android.view.MotionEvent
import android.view.View

class AlCropView : View {
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
        paint.color = Color.LTGRAY
        paint.strokeWidth = strokeWidth
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
        val ret = super.onTouchEvent(event)
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
                    return true
                }
                rect = RectF(srcRect)
                rect.offset(lt.x, rb.y)
                if (rect.contains(lastTouchPointF.x, lastTouchPointF.y)) {
                    loc = Loc.LB
                    return true
                }
                rect = RectF(srcRect)
                rect.offset(rb.x, rb.y)
                if (rect.contains(lastTouchPointF.x, lastTouchPointF.y)) {
                    loc = Loc.RB
                    return true
                }
                rect = RectF(srcRect)
                rect.offset(rb.x, lt.y)
                if (rect.contains(lastTouchPointF.x, lastTouchPointF.y)) {
                    loc = Loc.RT
                    return true
                }
                rect = RectF(lt.x, lt.y, rb.x, rb.y)
                if (rect.contains(lastTouchPointF.x, lastTouchPointF.y)) {
                    loc = Loc.C
                    return true
                }
            }
            MotionEvent.ACTION_MOVE -> {
                if (null != loc) {
                    val dx = event.x - lastTouchPointF.x
                    val dy = event.y - lastTouchPointF.y
                    when (loc) {
                        Loc.LT -> lt.offset(dx, dy)
                        Loc.LB -> {
                            lt.offset(dx, 0f)
                            rb.offset(0f, dy)
                        }
                        Loc.RB -> rb.offset(dx, dy)
                        Loc.RT -> {
                            rb.offset(dx, 0f)
                            lt.offset(0f, dy)
                        }
                        Loc.C -> {
                            lt.offset(dx, dy)
                            rb.offset(dx, dy)
                        }
                    }
                    lastTouchPointF.set(event.x, event.y)
                    onChangeListener?.onChange(this)
                    postInvalidate()
                    return true
                }
            }
        }
        return ret
    }

    override fun onDraw(canvas: Canvas?) {
        super.onDraw(canvas)
        lb.set(lt.x, rb.y)
        rt.set(rb.x, lt.y)
        drawLine(canvas, lt, lb, paint)
        drawLine(canvas, lb, rb, paint)
        drawLine(canvas, rb, rt, paint)
        drawLine(canvas, rt, lt, paint)

        //水平
        canvas?.drawRect(lt.x + hRectF.left - vRectF.width(), lt.y + hRectF.top - hRectF.height(),
                lt.x + hRectF.right - vRectF.width(), lt.y + hRectF.bottom - hRectF.height(), paint)
        //垂直
        canvas?.drawRect(lt.x + vRectF.left - vRectF.width(), lt.y + vRectF.top - hRectF.height(),
                lt.x + vRectF.right - vRectF.width(), lt.y + vRectF.bottom - hRectF.height(), paint)

        //水平
        canvas?.drawRect(lb.x + hRectF.left - vRectF.width(), lb.y + hRectF.top,
                lb.x + hRectF.right - vRectF.width(), lb.y + hRectF.bottom, paint)
        //垂直
        canvas?.drawRect(lb.x + vRectF.left - vRectF.width(), lb.y - vRectF.height() + hRectF.height(),
                lb.x + vRectF.right - vRectF.width(), lb.y + hRectF.height(), paint)

        //水平
        canvas?.drawRect(rb.x - hRectF.width() + vRectF.width(), rb.y,
                rb.x + vRectF.width(), rb.y + hRectF.height(), paint)
        //垂直
        canvas?.drawRect(rb.x, rb.y - vRectF.height() + hRectF.height(),
                rb.x + vRectF.width(), rb.y + hRectF.height(), paint)

        //水平
        canvas?.drawRect(rt.x + hRectF.left - hRectF.width() + vRectF.width(), rt.y + hRectF.top - hRectF.height(),
                rt.x + hRectF.right - hRectF.width() + vRectF.width(), rt.y + hRectF.bottom - hRectF.height(), paint)
        //垂直
        canvas?.drawRect(rt.x + vRectF.left, rt.y + vRectF.top - hRectF.height(),
                rt.x + vRectF.right, rt.y + vRectF.bottom - hRectF.height(), paint)
    }

    private fun drawLine(canvas: Canvas?, start: PointF, end: PointF, paint: Paint) {
        canvas?.drawLine(start.x, start.y, end.x, end.y, paint)
    }

    fun getCropRectF(): RectF = RectF(
            lt.x / (measuredWidth / 2f) - 1f,
            1f - lt.y / (measuredHeight / 2f),
            rb.x / (measuredWidth / 2f) - 1f,
            1f - rb.y / (measuredHeight / 2f))

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
}