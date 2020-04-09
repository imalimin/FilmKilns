package com.lmy.hwvcnative.widget

import android.content.Context
import android.graphics.*
import android.util.AttributeSet
import android.view.MotionEvent
import android.view.View
import android.view.WindowManager

class AlCropView : View {
    enum class Loc { LT, LB, RB, RT, C }

    private val lt = PointF()
    private val rb = PointF()
    private val lt0 = PointF()
    private val rb0 = PointF()
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
    private var fixAlign = false
    private val displaySize = Point()

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

        val wm = context.getSystemService(Context.WINDOW_SERVICE) as WindowManager
        wm.defaultDisplay.getRealSize(displaySize)
    }

    override fun onMeasure(widthMeasureSpec: Int, heightMeasureSpec: Int) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec)
        val width = View.MeasureSpec.getSize(widthMeasureSpec)
        val height = View.MeasureSpec.getSize(heightMeasureSpec)
        lt.x = width / 4f
        lt.y = height / 3f
        rb.x = width * 3 / 4f
        rb.y = height * 2 / 3f
        lt0.set(lt)
        rb0.set(rb)
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
                rect.offset(lt0.x, lt0.y)
                if (rect.contains(lastTouchPointF.x, lastTouchPointF.y)) {
                    loc = Loc.LT
                    return true
                }
                rect = RectF(srcRect)
                rect.offset(lt0.x, rb0.y)
                if (rect.contains(lastTouchPointF.x, lastTouchPointF.y)) {
                    loc = Loc.LB
                    return true
                }
                rect = RectF(srcRect)
                rect.offset(rb0.x, rb0.y)
                if (rect.contains(lastTouchPointF.x, lastTouchPointF.y)) {
                    loc = Loc.RB
                    return true
                }
                rect = RectF(srcRect)
                rect.offset(rb0.x, lt0.y)
                if (rect.contains(lastTouchPointF.x, lastTouchPointF.y)) {
                    loc = Loc.RT
                    return true
                }
                rect = RectF(lt0.x, lt0.y, rb0.x, rb0.y)
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
                        Loc.LT -> {
                            lt0.offset(dx, dy)
                            lt.set(lt0)
                            if (fixAlign) {
                                lt.x += ((rb0.x - lt0.x) - ((rb0.x - lt0.x) / 16).toInt() * 16f)
                                lt.y += ((rb0.y - lt0.y) - ((rb0.y - lt0.y) / 16).toInt() * 16f)
                            }
                        }
                        Loc.LB -> {
                            lt0.offset(dx, 0f)
                            rb0.offset(0f, dy)
                            lt.offset(dx, 0f)
                            rb.offset(0f, dy)
                        }
                        Loc.RB -> {
                            rb0.offset(dx, dy)
                            rb.offset(dx, dy)
                        }
                        Loc.RT -> {
                            rb0.offset(dx, 0f)
                            lt0.offset(0f, dy)
                            rb.offset(dx, 0f)
                            lt.offset(0f, dy)
                        }
                        Loc.C -> {
                            lt0.offset(dx, dy)
                            rb0.offset(dx, dy)
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
        canvas?.drawRect(
            lt.x + hRectF.left - vRectF.width(), lt.y + hRectF.top - hRectF.height(),
            lt.x + hRectF.right - vRectF.width(), lt.y + hRectF.bottom - hRectF.height(), paint
        )
        //垂直
        canvas?.drawRect(
            lt.x + vRectF.left - vRectF.width(), lt.y + vRectF.top - hRectF.height(),
            lt.x + vRectF.right - vRectF.width(), lt.y + vRectF.bottom - hRectF.height(), paint
        )

        //水平
        canvas?.drawRect(
            lb.x + hRectF.left - vRectF.width(), lb.y + hRectF.top,
            lb.x + hRectF.right - vRectF.width(), lb.y + hRectF.bottom, paint
        )
        //垂直
        canvas?.drawRect(
            lb.x + vRectF.left - vRectF.width(), lb.y - vRectF.height() + hRectF.height(),
            lb.x + vRectF.right - vRectF.width(), lb.y + hRectF.height(), paint
        )

        //水平
        canvas?.drawRect(
            rb.x - hRectF.width() + vRectF.width(), rb.y,
            rb.x + vRectF.width(), rb.y + hRectF.height(), paint
        )
        //垂直
        canvas?.drawRect(
            rb.x, rb.y - vRectF.height() + hRectF.height(),
            rb.x + vRectF.width(), rb.y + hRectF.height(), paint
        )

        //水平
        canvas?.drawRect(
            rt.x + hRectF.left - hRectF.width() + vRectF.width(),
            rt.y + hRectF.top - hRectF.height(),
            rt.x + hRectF.right - hRectF.width() + vRectF.width(),
            rt.y + hRectF.bottom - hRectF.height(),
            paint
        )
        //垂直
        canvas?.drawRect(
            rt.x + vRectF.left, rt.y + vRectF.top - hRectF.height(),
            rt.x + vRectF.right, rt.y + vRectF.bottom - hRectF.height(), paint
        )
    }

    private fun drawLine(canvas: Canvas?, start: PointF, end: PointF, paint: Paint) {
        canvas?.drawLine(start.x, start.y, end.x, end.y, paint)
    }

    fun getCropRectF(): RectF = RectF(
        lt.x / (measuredWidth / 2f) - 1f,
        1f - lt.y / (measuredHeight / 2f),
        rb.x / (measuredWidth / 2f) - 1f,
        1f - rb.y / (measuredHeight / 2f)
    )

    fun getCropRectFInDisplay(): RectF {
        val loc = IntArray(2)
        getLocationOnScreen(loc)
        val lt = PointF(loc[0] + this.lt.x, loc[1] + this.lt.y)
        val rb = PointF(loc[0] + this.rb.x, loc[1] + this.rb.y)
        return RectF(
            lt.x / (displaySize.x / 2f) - 1f,
            1f - lt.y / (displaySize.y / 2f),
            rb.x / (displaySize.x / 2f) - 1f,
            1f - rb.y / (displaySize.y / 2f)
        )
    }

    fun reset() {
        lt.x = measuredWidth / 4f
        lt.y = measuredHeight / 4f
        rb.x = measuredWidth * 3 / 4f
        rb.y = measuredHeight * 3 / 4f
        lt0.set(lt)
        rb0.set(rb)
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

    fun setFixAlign(fixAlign: Boolean) {
        this.fixAlign = fixAlign
    }

    interface OnChangeListener {
        fun onChange(view: View)
    }
}