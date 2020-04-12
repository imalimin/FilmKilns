package com.lmy.common.widget

import android.content.Context
import android.graphics.Rect
import android.os.Build
import android.text.TextUtils
import android.util.AttributeSet
import android.view.View
import android.widget.ImageButton
import android.widget.TextView
import androidx.appcompat.widget.ActionMenuView
import com.lmy.common.ext.locationInWindow
import com.lmy.common.ext.getStatusBarHeight

/**
 * Created by 李明艺 on 2016/8/26.
 *
 * @author aliminabc@gmail.com
 */
class Toolbar : androidx.appcompat.widget.Toolbar {
    private var mMenuView: ActionMenuView? = null
    private var mTitleTextView: TextView? = null
    private var mSubtitleTextView: TextView? = null
    private var mNavButtonView: ImageButton? = null
    private var shouldEnsure = false
    private var mStatusBarHeight = -1
    private var mOldHeight = -1

    constructor(context: Context) : super(context) {
        init()
    }

    constructor(context: Context, attrs: AttributeSet?) : super(context, attrs) {
        init()
    }

    constructor(context: Context, attrs: AttributeSet?, defStyleAttr: Int) : super(context, attrs, defStyleAttr) {
        init()
    }

    private fun init() {
        mStatusBarHeight = context.getStatusBarHeight()
    }

    override fun fitSystemWindows(insets: Rect): Boolean {
        return super.fitSystemWindows(insets)
    }

    private fun ensureMenuView() {
        try {
            if (null == mMenuView)
                mMenuView = getViewField("mMenuView") as ActionMenuView
            ensureView(mMenuView!!)
        } catch (e: NullPointerException) {
        }

    }

    private fun ensureNavButtonView() {
        try {
            if (null == mNavButtonView)
                mNavButtonView = getViewField("mNavButtonView") as ImageButton
            ensureView(mNavButtonView!!)
        } catch (e: NullPointerException) {
        }

    }

    private fun ensureView(view: View) {
        view.layout(view.left, mStatusBarHeight,
                view.right, bottom)
    }

    /**
     * 标题居中
     */
    private fun ensureTitleView() {
        try {
            if (mTitleTextView == null)
                mTitleTextView = getViewField("mTitleTextView") as TextView
            if (mTitleTextView == null)
                mSubtitleTextView = getViewField("mSubtitleTextView") as TextView
            if (null == mSubtitleTextView || TextUtils.isEmpty(mSubtitleTextView!!.text)) {
                val width = mTitleTextView!!.measuredWidth
                val height = mTitleTextView!!.measuredHeight
                val left = (measuredWidth - width) / 2
                val fixHeight = (measuredHeight - mStatusBarHeight - height) / 2
                mTitleTextView!!.layout(left, if (shouldEnsure) mStatusBarHeight + fixHeight else mTitleTextView!!.top,
                        left + width, if (shouldEnsure) mStatusBarHeight + fixHeight + height else mTitleTextView!!.bottom)
            }
        } catch (e: NullPointerException) {
            //            e.printStackTrace();
        }

    }

    override fun onMeasure(widthMeasureSpec: Int, heightMeasureSpec: Int) {
        var heightMeasureSpec = heightMeasureSpec
        if (shouldEnsure) {//与状态栏重合
            if (mOldHeight < 0)
                mOldHeight = View.MeasureSpec.getSize(heightMeasureSpec)
            heightMeasureSpec = View.MeasureSpec.makeMeasureSpec(mOldHeight + mStatusBarHeight,
                    View.MeasureSpec.getMode(heightMeasureSpec))
        }
        super.onMeasure(widthMeasureSpec, heightMeasureSpec)
    }

    override fun onLayout(changed: Boolean, l: Int, t: Int, r: Int, b: Int) {
        super.onLayout(changed, l, t, r, b)
        //        Log.v(Toolbar.class, "onLayout: " + changed + ", " + shouldEnsure);
        ensureTitleView()
        if (shouldEnsure) {
            ensureMenuView()
            ensureNavButtonView()
        }
    }

    override fun onSizeChanged(w: Int, h: Int, oldw: Int, oldh: Int) {
        super.onSizeChanged(w, h, oldw, oldh)
        if (!shouldEnsure)
        //唯一确定是否需要重新布局
            shouldEnsure = Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP && mStatusBarHeight > locationInWindow()[1]
    }

    @Throws(NullPointerException::class)
    private fun getViewField(fieldName: String): View {
        val obj = getField(fieldName)
        if (null != obj && obj is View)
            return obj
        throw NullPointerException("this view is null")
    }

    private fun getField(fieldName: String): Any? {
        try {
            val field = javaClass.superclass.getDeclaredField(fieldName)
            field.isAccessible = true
            return field.get(this)
        } catch (e: NoSuchFieldException) {
            e.printStackTrace()
        } catch (e: IllegalAccessException) {
            e.printStackTrace()
        }

        return null
    }
}
