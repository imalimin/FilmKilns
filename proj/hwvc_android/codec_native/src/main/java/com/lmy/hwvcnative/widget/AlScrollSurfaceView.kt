package com.lmy.hwvcnative.widget

import android.content.Context
import android.graphics.PointF
import android.support.v4.view.GestureDetectorCompat
import android.util.AttributeSet
import android.view.*
import android.widget.OverScroller
import com.lmy.hwvcnative.entity.AlRational
import kotlin.math.abs
import kotlin.math.atan2

class AlScrollSurfaceView : SurfaceView {
    private lateinit var mGestureDetector: GestureDetectorCompat
    private lateinit var mScaleDetector: ScaleGestureDetector
    private lateinit var scroller: OverScroller
    private var onScrollListener: OnScrollListener? = null
    private var onClickListener: OnClickListener? = null
    private var onScaleListener: OnScaleListener? = null
    private var onRotateListener: OnRotateListener? = null

    private val mCurrentPosition = PointF(0f, 0f)
    private var minFlingVelocity = 0
    private val minRotateVelocity = Math.PI / 180 / 2
    //Rotate gesture
    private var rotate = false
    private var startRotate = 0.0
    private var previousRotate = 0.0

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
        scroller = OverScroller(context)
        mGestureDetector = GestureDetectorCompat(context, mGestureListener)
        mScaleDetector = ScaleGestureDetector(context, mScaleListener)

        minFlingVelocity = ViewConfiguration.get(context).scaledMinimumFlingVelocity
    }

    override fun onTouchEvent(event: MotionEvent): Boolean {
        super.onTouchEvent(event)
        mScaleDetector.onTouchEvent(event)
        mGestureDetector.onTouchEvent(event)

        when (event.action and MotionEvent.ACTION_MASK) {
            MotionEvent.ACTION_POINTER_DOWN -> {
                parent.requestDisallowInterceptTouchEvent(true)
                rotate = true
                val spanX = event.getX(0) - event.getX(1)
                val spanY = event.getY(0) - event.getY(1)
                startRotate = atan2(spanY, spanX).toDouble()
                previousRotate = startRotate
            }
            MotionEvent.ACTION_POINTER_UP -> {
                rotate = false
            }
            MotionEvent.ACTION_UP, MotionEvent.ACTION_CANCEL -> {
                rotate = false
            }
            MotionEvent.ACTION_MOVE -> {
                if (rotate) {
                    val spanX = event.getX(0) - event.getX(1)
                    val spanY = event.getY(0) - event.getY(1)
                    val rotate = atan2(spanY, spanX).toDouble()
                    val delta = rotate - previousRotate
                    if (abs(delta) > minRotateVelocity) {
                        onRotateListener?.onRotate(this@AlScrollSurfaceView,
                                AlRational((delta * PRECISIONS).toInt(), PRECISIONS))
                        previousRotate = rotate
                    }
                }
            }
        }
        return true
    }

    fun setOnScrollListener(listener: (v: SurfaceView, x: Float, y: Float,
                                       dx: Float, dy: Float) -> Unit) {
        setOnScrollListener(object : OnScrollListener {
            override fun onScroll(v: SurfaceView, x: Float, y: Float, dx: Float, dy: Float) {
                listener(v, x, y, dx, dy)
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

    fun setOnScaleListener(listener: (v: SurfaceView, ds: AlRational) -> Unit) {
        setOnScaleListener(object : OnScaleListener {
            override fun onScale(v: SurfaceView, ds: AlRational) {
                listener(v, ds)
            }
        })
    }

    fun setOnScaleListener(listener: OnScaleListener) {
        onScaleListener = listener
    }

    fun setOnRotateListener(listener: (v: SurfaceView, dr: AlRational) -> Unit) {
        setOnRotateListener(object : OnRotateListener {
            override fun onRotate(v: SurfaceView, dr: AlRational) {
                listener(v, dr)
            }
        })
    }

    fun setOnRotateListener(listener: OnRotateListener) {
        onRotateListener = listener
    }

    interface OnScrollListener {
        fun onScroll(v: SurfaceView, x: Float, y: Float, dx: Float, dy: Float)
    }

    interface OnClickListener {
        fun onClick(v: SurfaceView, x: Float, y: Float)
    }

    interface OnScaleListener {
        /**
         * @param v SurfaceView
         * @param ds scale factor. currentSpan / previousSpan
         */
        fun onScale(v: SurfaceView, ds: AlRational)
    }

    interface OnRotateListener {
        fun onRotate(v: SurfaceView, dr: AlRational)
    }

    /**
     * +--------------------------------+
     * |  Tap or move gesture listener  |
     * +--------------------------------+
     */
    private val mGestureListener = object : GestureDetector.SimpleOnGestureListener() {
        override fun onDown(e: MotionEvent): Boolean {
            parent.requestDisallowInterceptTouchEvent(true)
            onClickListener?.onClick(this@AlScrollSurfaceView,
                    e.x * 2 / width.toFloat() - 1f,
                    -(e.y * 2 / height.toFloat() - 1f))
            return true
        }

        override fun onFling(e1: MotionEvent?, e2: MotionEvent?, velocityX: Float, velocityY: Float): Boolean {
            scroller.forceFinished(true)
            scroller.fling(mCurrentPosition.x.toInt(), mCurrentPosition.y.toInt(),
                    velocityX.toInt(), velocityY.toInt(), Integer.MAX_VALUE,
                    Integer.MAX_VALUE, 0, 0)
            return true
        }

        override fun onScroll(e1: MotionEvent?, e2: MotionEvent?, distanceX: Float, distanceY: Float): Boolean {
            //如果当前正在滚动，则停止滚动
            scroller.forceFinished(true)
            scroller.startScroll(scroller.currX, scroller.currY, distanceX.toInt(), distanceY.toInt())
//        mCurrentPosition.x = scroller.currX.toFloat()
//        mCurrentPosition.y = scroller.currY.toFloat()
            mCurrentPosition.x -= distanceX
            mCurrentPosition.y -= distanceY
            onScrollListener?.onScroll(this@AlScrollSurfaceView,
                    mCurrentPosition.x, mCurrentPosition.y,
                    -distanceX / width.toFloat() * 2f, distanceY / height.toFloat() * 2f)
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
            val num = (detector.scaleFactor * PRECISIONS).toInt()
            val den = (previousScaleFactor * PRECISIONS).toInt()
            onScaleListener?.onScale(this@AlScrollSurfaceView, AlRational(num, den))
            previousScaleFactor = detector.scaleFactor
            return super.onScale(detector)
        }
    }

    companion object {
        const val PRECISIONS = 100000
    }
}