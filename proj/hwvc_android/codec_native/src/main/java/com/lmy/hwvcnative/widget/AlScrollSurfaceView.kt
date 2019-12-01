package com.lmy.hwvcnative.widget

import android.content.Context
import android.graphics.PointF
import android.support.v4.view.GestureDetectorCompat
import android.util.AttributeSet
import android.view.GestureDetector
import android.view.MotionEvent
import android.view.SurfaceView
import android.view.ViewConfiguration
import android.widget.OverScroller

class AlScrollSurfaceView : SurfaceView, GestureDetector.OnGestureListener {
    private lateinit var mGestureDetector: GestureDetectorCompat
    private lateinit var scroller: OverScroller
    private var onScrollListener: OnScrollListener? = null
    private var onPosClickListener: OnPosClickListener? = null

    private val mCurrentPosition = PointF(0f, 0f)
    private var minFlingVelocity = 0

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
        mGestureDetector = GestureDetectorCompat(context, this)

        minFlingVelocity = ViewConfiguration.get(context).scaledMinimumFlingVelocity
    }

    override fun onTouchEvent(event: MotionEvent): Boolean {
        val result = mGestureDetector.onTouchEvent(event)
        //如果手指离开屏幕，并且没有惯性滑动
//        if (event.action == MotionEvent.ACTION_UP && !fling) {
//            mCurrentScrollDirection = RulerView.Direction.NONE
//        }
        return result
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

    fun setOnPosClickListener(listener: (v: SurfaceView, x: Float, y: Float) -> Unit) {
        setOnPosClickListener(object : OnPosClickListener {
            override fun onClick(v: SurfaceView, x: Float, y: Float) {
                listener(v, x, y)
            }
        })
    }

    fun setOnPosClickListener(listener: OnPosClickListener?) {
        onPosClickListener = listener
    }

    /**
     * +---------------------+
     * |  OnGestureListener  |
     * +---------------------+
     */
    override fun onShowPress(e: MotionEvent?) {
    }

    override fun onSingleTapUp(e: MotionEvent?): Boolean {
        return false
    }

    override fun onDown(e: MotionEvent): Boolean {
        parent.requestDisallowInterceptTouchEvent(true)
        onPosClickListener?.onClick(this,
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
        onScrollListener?.onScroll(this,
                mCurrentPosition.x, mCurrentPosition.y,
                -distanceX / width.toFloat() * 2f, distanceY / height.toFloat() * 2f)
        return true
    }

    override fun onLongPress(e: MotionEvent?) {
    }

    interface OnScrollListener {
        fun onScroll(v: SurfaceView, x: Float, y: Float, dx: Float, dy: Float)
    }

    interface OnPosClickListener {
        fun onClick(v: SurfaceView, x: Float, y: Float)
    }
}