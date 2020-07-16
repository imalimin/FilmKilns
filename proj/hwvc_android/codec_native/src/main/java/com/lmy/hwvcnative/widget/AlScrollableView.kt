package com.lmy.hwvcnative.widget

import android.content.Context
import android.graphics.PointF
import android.util.AttributeSet
import android.util.Log
import android.view.GestureDetector
import android.view.MotionEvent
import android.view.ViewConfiguration
import android.widget.OverScroller
import androidx.annotation.AttrRes
import androidx.core.view.GestureDetectorCompat
import androidx.core.view.ViewCompat
import kotlin.math.abs

open class AlScrollableView : AlAbsView {
    enum class Direction {
        NONE, VERTICAL
    }

    //滚动控制器
    private lateinit var scroller: OverScroller
    private var maxFlingVelocity = 0
    private var minFlingVelocity = 0
    private var touchSlop = 0

    //当前滚动方向
    private var mCurrentScrollDirection = Direction.NONE

    //当前惯性滚动方向
    private var mCurrentFlingDirection = Direction.NONE

    //当前滚动x,y
    private val mCurrentOrigin = PointF(0f, 0f)

    //手势支持
    private lateinit var mGestureDetector: GestureDetectorCompat

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
        scroller = OverScroller(context)
        mGestureDetector = GestureDetectorCompat(context, onGestureListener)
        maxFlingVelocity = ViewConfiguration.get(context).scaledMaximumFlingVelocity
        minFlingVelocity = ViewConfiguration.get(context).scaledMinimumFlingVelocity
        touchSlop = ViewConfiguration.get(context).scaledTouchSlop

    }

    fun getScrollPosition(): PointF = mCurrentOrigin

    override fun onTouchEvent(event: MotionEvent): Boolean {
        val result = mGestureDetector.onTouchEvent(event)
        //如果手指离开屏幕，并且没有惯性滑动
        if (event.action == MotionEvent.ACTION_UP && mCurrentFlingDirection == Direction.NONE) {
            if (mCurrentScrollDirection == Direction.VERTICAL) {
                onScrollDone(getScrollPosition())
            }
            mCurrentScrollDirection = Direction.NONE
        }
        return result
    }

    private fun checkOriginY() {
        if (mCurrentOrigin.y > 0) mCurrentOrigin.y = 0f
        if (mCurrentOrigin.y < measuredHeight - measuredHeight * 2)
            mCurrentOrigin.y = (measuredHeight - measuredHeight * 2).toFloat()
    }

    open fun onScroll(pos: PointF, dx: Float, dy: Float) {

    }

    open fun onScrollDone(pos: PointF) {

    }

    open fun onFling(pos: PointF, dx: Float, dy: Float) {

    }

    open fun onFlingDone(pos: PointF) {

    }

    /**
     * 手势监听
     */
    private val onGestureListener = object : GestureDetector.SimpleOnGestureListener() {
        /**
         * 手指按下回调，这里将状态标记为非滚动状态
         */
        override fun onDown(e: MotionEvent): Boolean {
            parent.requestDisallowInterceptTouchEvent(true)
            mCurrentScrollDirection = Direction.NONE
            return true
        }

        /**
         * 手指拖动回调
         */
        override fun onScroll(
            e1: MotionEvent,
            e2: MotionEvent,
            distanceX: Float,
            distanceY: Float
        ): Boolean {
            //如果当前正在滚动，则停止滚动
            scroller.forceFinished(true)
            if (Direction.NONE == mCurrentScrollDirection) {//判断滚动方向，这里只有垂直一个方向
                mCurrentScrollDirection = if (abs(distanceX) < abs(distanceY)) {
                    Direction.VERTICAL
                } else {
                    Direction.NONE
                }
            }
            // Calculate the new origin after scroll.
            when (mCurrentScrollDirection) {
                Direction.VERTICAL -> {//计算手指拖动距离，并记录新的坐标重绘界面
                    mCurrentOrigin.y -= distanceY
                    checkOriginY()
                    ViewCompat.postInvalidateOnAnimation(this@AlScrollableView)
                }
            }
            onScroll(getScrollPosition(), 0f, 0f)
            return true
        }

        /**
         * 惯性滚动回调
         */
        override fun onFling(
            e1: MotionEvent,
            e2: MotionEvent,
            velocityX: Float,
            velocityY: Float
        ): Boolean {
            scroller.forceFinished(true)
            mCurrentFlingDirection = mCurrentScrollDirection
            when (mCurrentFlingDirection) {
                Direction.VERTICAL -> scroller.fling(
                    mCurrentOrigin.x.toInt(), mCurrentOrigin.y.toInt(),
                    0, velocityY.toInt(), Integer.MIN_VALUE,
                    Integer.MAX_VALUE, Integer.MIN_VALUE, 0
                )
            }
            onFling(getScrollPosition(), 0f, 0f)
            ViewCompat.postInvalidateOnAnimation(this@AlScrollableView)
            return true
        }
    }

    companion object {
        const val TAG = "AlTrackView"
    }
}