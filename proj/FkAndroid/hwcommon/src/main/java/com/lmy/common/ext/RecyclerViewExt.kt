package com.lmy.common.ext

import android.content.Context
import android.graphics.Rect
import android.view.GestureDetector
import android.view.MotionEvent
import android.view.View
import android.view.ViewGroup
import androidx.recyclerview.widget.RecyclerView

fun RecyclerView.setOnItemClickListener(handler: (parent: RecyclerView, view: View?, position: Int) -> Unit) {
    addOnItemTouchListener(OnRecyclerItemClickListener(context,
            object : OnRecyclerItemClickListener.OnItemClickListener {
                override fun onItemClick(parent: RecyclerView, view: View?, position: Int) {
                    handler(parent, view, position)
                }
            }))
}

fun RecyclerView.setOnItemLongClickListener(handler: (parent: RecyclerView, view: View?, position: Int) -> Unit) {
    addOnItemTouchListener(OnRecyclerItemClickListener(context,
            object : OnRecyclerItemClickListener.OnItemLongClickListener {
                override fun onItemClick(parent: RecyclerView, view: View?, position: Int) {
                }

                override fun onLongItemClick(parent: RecyclerView, view: View?, position: Int) {
                    handler(parent, view, position)
                }
            }))
}

class OnRecyclerItemClickListener(context: Context, var mListener: OnItemClickListener?) : RecyclerView.OnItemTouchListener {
    private lateinit var recyclerView: RecyclerView
    private var mGestureDetector: GestureDetector

    interface OnItemClickListener {
        fun onItemClick(parent: RecyclerView, view: View?, position: Int)
    }

    interface OnItemLongClickListener : OnItemClickListener {
        fun onLongItemClick(parent: RecyclerView, view: View?, position: Int)
    }

    interface OnItemDoubleClickListener : OnItemLongClickListener {
        fun onItemDoubleClick(parent: RecyclerView, view: View?, position: Int)
    }

    init {
        mGestureDetector = createGestureDetector(context)
    }

    private fun hasChildViewOnClickListeners(view: View?, x: Int, y: Int): Boolean {
        if (view !is ViewGroup) return view!!.hasOnClickListeners()
        val viewGroup = view as ViewGroup?
        for (i in viewGroup!!.childCount - 1 downTo 0) {
            val v = viewGroup.getChildAt(i)
            val rect = Rect()
            v.getHitRect(rect)
            if (!rect.contains(x, y)) continue
            //            Log.v(OnRecyclerItemClickListener.class, v.getClass().getSimpleName(), v.hasOnClickListeners());
            if (v.hasOnClickListeners()) return true
            if (v is ViewGroup && hasChildViewOnClickListeners(v, x - v.getLeft(), y - v.getTop()))
                return true
        }
        return false
    }

    private fun createGestureDetector(context: Context): GestureDetector {
        return GestureDetector(context, object : GestureDetector.SimpleOnGestureListener() {
            override fun onSingleTapUp(e: MotionEvent): Boolean {
                val childView = recyclerView.findChildViewUnder(e.x, e.y)
                //                Log.v(OnRecyclerItemClickListener.class, e.getX(), e.getY());
                if (childView != null && mListener != null && !hasChildViewOnClickListeners(childView,
                                e.x.toInt() - childView.left, e.y.toInt() - childView.top))
                    mListener!!.onItemClick(recyclerView, childView, recyclerView.getChildAdapterPosition(childView))
                return false
            }

            override fun onLongPress(e: MotionEvent) {
                val childView = recyclerView.findChildViewUnder(e.x, e.y)
                if (childView != null && mListener != null && !hasChildViewOnClickListeners(childView,
                                e.x.toInt() - childView.left, e.y.toInt() - childView.top))
                    if (mListener is OnItemLongClickListener)
                        (mListener as OnItemLongClickListener).onLongItemClick(recyclerView, childView, recyclerView.getChildAdapterPosition(childView))
            }

            override fun onDoubleTap(e: MotionEvent): Boolean {
                val childView = recyclerView.findChildViewUnder(e.x, e.y)
                if (childView != null && mListener != null && !hasChildViewOnClickListeners(childView,
                                e.x.toInt() - childView.left, e.y.toInt() - childView.top))
                    if (mListener is OnItemDoubleClickListener)
                        (mListener as OnItemDoubleClickListener).onItemDoubleClick(recyclerView, childView, recyclerView.getChildAdapterPosition(childView))
                return true
            }
        })
    }

    override fun onInterceptTouchEvent(view: RecyclerView, e: MotionEvent): Boolean {
        this.recyclerView = view
        return if (view.scrollState != RecyclerView.SCROLL_STATE_IDLE) false else mGestureDetector.onTouchEvent(e)
    }

    override fun onTouchEvent(view: RecyclerView, motionEvent: MotionEvent) {}

    override fun onRequestDisallowInterceptTouchEvent(disallowIntercept: Boolean) {}
}