package com.lmy.common.widget.behavior

import android.content.Context
import android.support.design.widget.CoordinatorLayout
import android.support.v7.widget.RecyclerView
import android.util.AttributeSet
import android.view.View
import com.lmy.common.widget.ViewCompat

/**
 * @author limingyi
 * Created on 2017/6/3 0003 17:08.
 */
class ScrollOutBehavior(context: Context, attrs: AttributeSet) : CoordinatorLayout.Behavior<View>(context, attrs) {
    var mScrollUp = false
    var animate: ViewCompat.ViewValueAnimator? = null

    override fun layoutDependsOn(parent: CoordinatorLayout, child: View, dependency: View): Boolean {
        return dependency is RecyclerView
    }

    override fun onStartNestedScroll(coordinatorLayout: CoordinatorLayout, child: View, directTargetChild: View, target: View, axes: Int, type: Int): Boolean {
        //处理垂直方向上的滚动事件
        return axes == android.support.v4.view.ViewCompat.SCROLL_AXIS_VERTICAL || super.onStartNestedScroll(coordinatorLayout, child, directTargetChild, target, axes)
    }

    override fun onNestedScroll(parent: CoordinatorLayout, child: View, target: View, dxConsumed: Int, dyConsumed: Int, dxUnconsumed: Int, dyUnconsumed: Int) {
        super.onNestedScroll(parent, child, target, dxConsumed, dyConsumed, dxUnconsumed, dyUnconsumed)
        if (dyConsumed == 0) return
        val scrollUp = dyConsumed > 0
        if (mScrollUp == scrollUp)
            return
        mScrollUp = scrollUp
        animate = ViewCompat.animate(child).translationY((if (mScrollUp) parent.bottom - child.top else 0).toFloat())
                .setDuration(parent.context!!.resources.getInteger(android.R.integer.config_shortAnimTime).toLong())
        animate!!.start()
    }
}