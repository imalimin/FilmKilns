package com.lmy.samplenative.ui

import android.content.Context
import android.graphics.PixelFormat
import android.graphics.PointF
import android.graphics.RectF
import android.os.Build
import android.util.Log
import android.util.Size
import android.view.*
import com.lmy.common.ext.locationInWindow
import com.lmy.samplenative.R


class FloatWindow(private val ctx: Context) : View.OnClickListener, View.OnTouchListener {
    private var wm: WindowManager? = null
    private var view: View? = null
    private var lp: WindowManager.LayoutParams? = null
    private var size: Size

    private var frameView: View? = null
    private var startBtn: View? = null
    private var closeBtn: View? = null
    private var adjustSize: Int = 0

    init {
        wm = ctx.getSystemService(Context.WINDOW_SERVICE) as WindowManager
        val metrics = ctx.resources.displayMetrics
        size = Size(metrics.widthPixels, metrics.heightPixels)
        adjustSize = (size.width * 0.1f).toInt()
        lp = WindowManager.LayoutParams().apply {
            format = PixelFormat.RGBA_8888
            flags = WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE
            gravity = Gravity.LEFT.or(Gravity.TOP)
            width = size.width
            height = size.width
            type = if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY
            } else {
                WindowManager.LayoutParams.TYPE_PHONE
            }
        }
        view = LayoutInflater.from(ctx).inflate(R.layout.win_float, null)
        frameView = view?.findViewById(R.id.frameView)
        startBtn = view?.findViewById(R.id.startBtn)
        closeBtn = view?.findViewById(R.id.closeBtn)
        view?.setOnTouchListener(this)
        startBtn?.setOnClickListener(this)
        closeBtn?.setOnClickListener(this)
    }

    fun show() {
        view?.visibility = View.VISIBLE
        wm?.addView(view, lp)
    }

    fun dismiss() {
        view?.visibility = View.GONE
        wm?.removeView(view)
    }

    fun getRect(): RectF {
        val loc = IntArray(2)
        view!!.getLocationOnScreen(loc)
        val rect = RectF()
        rect.left = loc[0] / (size.width / 2f) - 1f
        rect.top = 1f - loc[1] / (size.height / 2f)
        rect.right = rect.left + (frameView!!.measuredWidth / (size.width / 2f))
        rect.bottom = rect.top - (frameView!!.measuredHeight / (size.height / 2f))
        return rect
    }

    override fun onClick(v: View) {
        when (v.id) {
            R.id.startBtn -> {
                Log.i("FloatWindow", getRect().toString())
            }
            R.id.closeBtn -> {
                dismiss()
            }
        }
    }

    private var loc: Int = LOC_IDL
    private val pointF = PointF()
    override fun onTouch(v: View, event: MotionEvent): Boolean {
        when (event.action) {
            MotionEvent.ACTION_DOWN -> {
                pointF.x = event.rawX
                pointF.y = event.rawY
                loc = getLoc(frameView!!, event)
            }
            MotionEvent.ACTION_MOVE -> {
                val dx = (event.rawX - pointF.x).toInt()
                val dy = (event.rawY - pointF.y).toInt()
                when (loc) {
                    LOC_LT -> {
                        lp!!.width -= dx
                        lp!!.height -= dy
                        lp!!.x += dx
                        lp!!.y += dy
                    }
                    LOC_RT -> {
                        lp!!.width += dx
                        lp!!.height -= dy
                        lp!!.y += dy
                    }
                    LOC_RB -> {
                        lp!!.width += dx
                        lp!!.height += dy
                    }
                    LOC_LB -> {
                        lp!!.width -= dx
                        lp!!.height += dy
                        lp!!.x += dx
                    }
                    LOC_IDL -> {
                        lp!!.x += dx
                        lp!!.y += dy
                    }
                }
                pointF.x = event.rawX
                pointF.y = event.rawY
                lp!!.x = Math.max(0, lp!!.x)
                lp!!.x = Math.min(size.width, lp!!.x)
                lp!!.y = Math.max(0, lp!!.y)
                lp!!.y = Math.min(size.height, lp!!.y)

                lp!!.width = Math.max(adjustSize * 2, lp!!.width)
                lp!!.width = Math.min(size.width, lp!!.width)
                lp!!.height = Math.max(adjustSize * 2, lp!!.height)
                lp!!.height = Math.min(size.height, lp!!.height)
                wm?.updateViewLayout(view, lp)
            }
            MotionEvent.ACTION_UP or MotionEvent.ACTION_CANCEL -> {
                pointF.x = 0f
                pointF.y = 0f
                loc = LOC_IDL
            }
        }
        return false
    }

    private fun getLoc(v: View, event: MotionEvent): Int {
        if (event.x >= 0 && event.x <= adjustSize && event.y >= 0 && event.y <= adjustSize) {
            return LOC_LT
        }
        if (event.x <= v.width && v.width - event.x <= adjustSize && event.y >= 0 && event.y <= adjustSize) {
            return LOC_RT
        }
        if (event.x <= v.width && v.width - event.x <= adjustSize
                && event.y <= v.height && v.height - event.y <= adjustSize) {
            return LOC_RB
        }
        if (event.x >= 0 && event.x <= adjustSize
                && event.y <= v.height && v.height - event.y <= adjustSize) {
            return LOC_LB
        }
        return LOC_IDL
    }

    companion object {
        const val LOC_IDL = 0
        const val LOC_LT = 1
        const val LOC_RT = 2
        const val LOC_RB = 3
        const val LOC_LB = 4
    }
}