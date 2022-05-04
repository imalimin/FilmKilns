package com.alimin.fk.widgets

import android.content.Context
import android.graphics.Point
import android.graphics.PointF
import android.util.AttributeSet
import android.view.*
import android.widget.OverScroller
import androidx.core.view.GestureDetectorCompat
import com.alimin.fk.entity.FkRational
import kotlin.math.abs
import kotlin.math.atan2

class FkActSurfaceView : SurfaceView {
    enum class ACTION {
        IDL, SCALE, ROTATE, MOVE
    }

    private lateinit var mGestureDetector: GestureDetectorCompat
    private lateinit var mScaleDetector: ScaleGestureDetector
    private lateinit var scroller: OverScroller
    private var onScrollListener: OnScrollListener? = null
    private var onClickListener: OnClickListener? = null
    private var onScaleListener: OnScaleListener? = null
    private var onRotateListener: OnRotateListener? = null
    private var onActionListener: OnActionListener? = null

    private val mCurrentPosition = PointF(0f, 0f)
    private var minFlingVelocity = 0
    private val minRotateVelocity = Math.PI / 180 / 2
    private var minScaleVelocity = 0.02
    //Rotate gesture
    private var isDoublePointer = false
    private var previousRotate = Double.MAX_VALUE
    private var action: ACTION = ACTION.IDL
    private val lastFlingPoint = Point(0, 0)
    private val mChoreographer = Choreographer.getInstance()
    private lateinit var vSyncCallback: Choreographer.FrameCallback

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
        vSyncCallback = object : Choreographer.FrameCallback {
            override fun doFrame(frameTimeNanos: Long) {
                forceRender()
            }
        }
        scroller = OverScroller(context).apply {
            setFriction(1f)
        }
        mGestureDetector = GestureDetectorCompat(context, mGestureListener)
        mScaleDetector = ScaleGestureDetector(context, mScaleListener)

        minFlingVelocity = ViewConfiguration.get(context).scaledMinimumFlingVelocity
    }

    private fun notifyRender() {
        mChoreographer.removeFrameCallback(vSyncCallback)
        mChoreographer.postFrameCallback(vSyncCallback)
    }

    private fun forceRender() {
        onActionListener?.onRender(this)
    }

    override fun onTouchEvent(event: MotionEvent): Boolean {
        super.onTouchEvent(event)
        mScaleDetector.onTouchEvent(event)
        mGestureDetector.onTouchEvent(event)

        when (event.action and MotionEvent.ACTION_MASK) {
            MotionEvent.ACTION_POINTER_DOWN -> {
                parent.requestDisallowInterceptTouchEvent(true)
                isDoublePointer = true
            }
            MotionEvent.ACTION_POINTER_UP -> {
                isDoublePointer = false
                action = ACTION.IDL
                previousRotate = Double.MAX_VALUE
                forceRender()
            }
            MotionEvent.ACTION_UP, MotionEvent.ACTION_CANCEL -> {
                isDoublePointer = false
                action = ACTION.IDL
                previousRotate = Double.MAX_VALUE
                val xy = PointF(
                    event.x * 2 / measuredWidth.toFloat() - 1f,
                    -(event.y * 2 / measuredHeight.toFloat() - 1f)
                )
                onScrollListener?.onScroll(
                    this@FkActSurfaceView, xy.x, xy.y,
                    0f, 0f, 0
                )
                forceRender()
            }
            MotionEvent.ACTION_MOVE -> {
                if (isDoublePointer) {
                    val spanX = event.getX(0) - event.getX(1)
                    val spanY = event.getY(0) - event.getY(1)
                    val rotate = Math.PI + atan2(spanY, spanX).toDouble()
                    if (abs(previousRotate - rotate) > Math.PI) {
                        previousRotate = Double.MAX_VALUE
                    }
                    if (previousRotate != Double.MAX_VALUE) {
                        val delta = previousRotate - rotate
                        if (abs(delta) > minRotateVelocity && ACTION.IDL == action) {
                            action = ACTION.ROTATE
                        }
                        onRotateListener?.onRotate(
                            this@FkActSurfaceView,
                            FkRational((delta * PRECISIONS).toInt(), PRECISIONS),
                            getAnchor(event)
                        )
                        notifyRender()
                    }
                    previousRotate = rotate
                }
            }
        }
        return true
    }

    private fun getAnchor(event: MotionEvent) :PointF {
        val anchor = PointF(
            (event.getX(0) + event.getX(1)) / 2.0f,
            (event.getY(0) + event.getY(1)) / 2.0f
        )
        anchor.x = anchor.x * 2 / measuredWidth.toFloat() - 1f
        anchor.y = -(anchor.y * 2 / measuredHeight.toFloat() - 1f)
        return anchor
    }

    fun setOnScrollListener(
        listener: (
            v: SurfaceView, x: Float, y: Float,
            dx: Float, dy: Float, status: Int
        ) -> Unit
    ) {
        setOnScrollListener(object : OnScrollListener {
            override fun onScroll(
                v: SurfaceView, x: Float, y: Float,
                dx: Float, dy: Float, status: Int
            ) {
                listener(v, x, y, dx, dy, status)
            }
        })
    }

    fun setOnScrollListener(listener: OnScrollListener?) {
        onScrollListener = listener
    }

    fun setOnClickListener(listener: (v: SurfaceView, x: Float, y: Float) -> Unit) {
        setOnClickListener(object : OnClickListener {
            override fun onClick(v: SurfaceView, x: Float, y: Float) {
                listener(v, x, y)
            }
        })
    }

    fun setOnClickListener(listener: OnClickListener?) {
        onClickListener = listener
    }

    fun setOnScaleListener(listener: (v: SurfaceView, ds: FkRational, anchor: PointF) -> Unit) {
        setOnScaleListener(object : OnScaleListener {
            override fun onScale(v: SurfaceView, ds: FkRational, anchor: PointF) {
                listener(v, ds, anchor)
            }
        })
    }

    fun setOnScaleListener(listener: OnScaleListener) {
        onScaleListener = listener
    }

    fun setOnRotateListener(listener: (v: SurfaceView, dr: FkRational, anchor: PointF) -> Unit) {
        setOnRotateListener(object : OnRotateListener {
            override fun onRotate(v: SurfaceView, dr: FkRational, anchor: PointF) {
                listener(v, dr, anchor)
            }
        })
    }

    fun setOnRotateListener(listener: OnRotateListener) {
        onRotateListener = listener
    }

    fun setOnActionListener(listener: OnActionListener) {
        onActionListener = listener
    }

    interface OnScrollListener {
        /**
         * @param status 0:IDL,1:SCROLL
         */
        fun onScroll(v: SurfaceView, x: Float, y: Float, dx: Float, dy: Float, status: Int)
    }

    interface OnClickListener {
        fun onClick(v: SurfaceView, x: Float, y: Float)
    }

    interface OnScaleListener {
        /**
         * @param v SurfaceView
         * @param ds scale factor. currentSpan / previousSpan
         */
        fun onScale(v: SurfaceView, ds: FkRational, anchor: PointF)
    }

    interface OnRotateListener {
        fun onRotate(v: SurfaceView, dr: FkRational, anchor: PointF)
    }

    interface OnActionListener {
        fun onRender(v: SurfaceView)
    }

    override fun computeScroll() {
        super.computeScroll()
        if (scroller.computeScrollOffset()) {
            onScrollListener?.onScroll(
                this@FkActSurfaceView, mCurrentPosition.x, mCurrentPosition.y,
                (scroller.currX - lastFlingPoint.x).toFloat(),
                (scroller.currY - lastFlingPoint.y).toFloat(), 1
            )
            notifyRender()
            lastFlingPoint.set(scroller.currX, scroller.currY)
            postInvalidate()
        }
    }

    /**
     * +--------------------------------+
     * |  Tap or move gesture listener  |
     * +--------------------------------+
     */
    private val mGestureListener = object : GestureDetector.SimpleOnGestureListener() {
        override fun onDown(e: MotionEvent): Boolean {
            parent.requestDisallowInterceptTouchEvent(true)
            onClickListener?.onClick(
                this@FkActSurfaceView,
                e.x * 2 / width.toFloat() - 1f,
                -(e.y * 2 / height.toFloat() - 1f)
            )
            return true
        }

        override fun onFling(
            e1: MotionEvent,
            e2: MotionEvent,
            velocityX: Float,
            velocityY: Float
        ): Boolean {
            if (isDoublePointer) {
                return true
            }
            scroller.forceFinished(true)
            lastFlingPoint.set(scroller.currX, scroller.currY)
            scroller.fling(
                lastFlingPoint.x, lastFlingPoint.y,
                velocityX.toInt(), velocityY.toInt(), -1000,
                1000, -1000, 1000
            )
            postInvalidate()
            return super.onFling(e1, e2, velocityX, velocityY)
        }

        override fun onScroll(
            e1: MotionEvent,
            e2: MotionEvent,
            distanceX: Float,
            distanceY: Float
        ): Boolean {
            if (isDoublePointer) {
                return true
            }
            //如果当前正在滚动，则停止滚动
            scroller.forceFinished(true)
            scroller.startScroll(
                scroller.currX,
                scroller.currY,
                distanceX.toInt(),
                distanceY.toInt()
            )
            mCurrentPosition.x -= distanceX
            mCurrentPosition.y -= distanceY
            onScrollListener?.onScroll(
                this@FkActSurfaceView, mCurrentPosition.x, mCurrentPosition.y,
                -distanceX, -distanceY, 1
            )
            notifyRender()
            return true
        }
    }

    /**
     * +--------------------------------+
     * |  Scale gesture listener  |
     * +--------------------------------+
     */
    private val mScaleListener = object : ScaleGestureDetector.SimpleOnScaleGestureListener() {
        private var previousScaleFactor = 1f
        override fun onScaleBegin(detector: ScaleGestureDetector): Boolean {
            previousScaleFactor = 1f
            return super.onScaleBegin(detector)
        }

        override fun onScale(detector: ScaleGestureDetector): Boolean {
            if (ACTION.IDL == action) {
                action = ACTION.SCALE
            }
//            if (ACTION.SCALE != action) {
//                return super.onScale(detector)
//            }
            val num = (detector.scaleFactor * PRECISIONS).toInt()
            val den = (previousScaleFactor * PRECISIONS).toInt()
            val anchor = PointF(
                detector.focusX * 2 / measuredWidth.toFloat() - 1f,
                -(detector.focusY * 2 / measuredHeight.toFloat() - 1f)
            )
            onScaleListener?.onScale(this@FkActSurfaceView, FkRational(num, den), anchor)
            previousScaleFactor = detector.scaleFactor
            notifyRender()
            return super.onScale(detector)
        }
    }

    companion object {
        const val PRECISIONS = 100000000
    }
}