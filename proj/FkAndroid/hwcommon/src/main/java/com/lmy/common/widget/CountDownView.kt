package com.lmy.common.widget

import android.content.Context
import android.os.CountDownTimer
import android.text.TextUtils
import android.util.AttributeSet
import android.widget.TextView
import androidx.annotation.AttrRes
import com.lmy.common.R

class CountDownView : TextView {

    constructor(context: Context) : this(context, null)

    constructor(context: Context, attrs: AttributeSet?)
            : this(context, attrs, 0)

    constructor(context: Context, attrs: AttributeSet?, @AttrRes defStyleAttr: Int)
            : this(context, attrs, defStyleAttr, 0)

    constructor(context: Context, attrs: AttributeSet?, @AttrRes defStyleAttr: Int, defStyleRes: Int)
            : super(context, attrs, defStyleAttr) {
        resolveAttribute(context, attrs, defStyleAttr, defStyleRes)
    }

    private var defText: CharSequence = "SEND"
    private var countTextFormat: String = "%dS"
    private var counter: CountDownTimer? = null
    private fun resolveAttribute(context: Context, attrs: AttributeSet?, defStyleAttr: Int, defStyleRes: Int) {
        defText = text
        val a = context.theme.obtainStyledAttributes(attrs, R.styleable.app, defStyleAttr, defStyleRes)
        var i = 0
        val count = a.indexCount
        while (i < count) {
            val attr = a.getIndex(i)
            when (attr) {
                R.styleable.app_countTextFormat -> {
                    val s = a.getString(attr)
                    if (!TextUtils.isEmpty(s)) {
                        countTextFormat = s!!
                    }
                }
            }
            i++
        }
        a.recycle()
    }

    fun count() {
        count(60)
    }

    fun count(sec: Int) {
        isEnabled = false
        isClickable = false
        counter = object : CountDownTimer(sec * 1000L, 1000) {

            override fun onTick(millisUntilFinished: Long) {
                text = String.format(countTextFormat, millisUntilFinished / 1000)
                isEnabled = false
                isClickable = false
            }

            override fun onFinish() {
                text = defText
                isEnabled = true
                isClickable = true
            }
        }
        counter?.start()
    }

    override fun onDetachedFromWindow() {
        super.onDetachedFromWindow()
        counter?.onFinish()
        counter = null
    }
}