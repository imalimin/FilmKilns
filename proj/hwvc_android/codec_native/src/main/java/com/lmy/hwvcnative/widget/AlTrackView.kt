package com.lmy.hwvcnative.widget

import android.content.Context
import android.content.res.Resources
import android.graphics.Color
import android.graphics.Paint
import android.util.AttributeSet
import android.util.DisplayMetrics
import android.util.Log
import android.util.TypedValue
import android.widget.LinearLayout
import android.widget.TextView
import androidx.annotation.AttrRes
import com.lmy.hwvcnative.entity.AlMediaTrack
import com.lmy.hwvcnative.entity.AlMediaType
import com.lmy.hwvcnative.entity.AlRational
import java.util.*

class AlTrackView : LinearLayout {
    private val paint = Paint(Paint.ANTI_ALIAS_FLAG)
    private var durationInUS = 0L
    private val scale = AlRational(1, 1)
    private val map = TreeMap<AlMediaTrack, TextView>()

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
        orientation = VERTICAL
        paint.strokeWidth = 3f
        paint.color = Color.RED
    }

    fun setDuration(us: Long) {
        this.durationInUS = us
    }

    fun addTrack(track: AlMediaTrack) {
        if (map.containsKey(track)) {
            return
        }

        val r = if (context == null) {
            Resources.getSystem()
        } else {
            context.resources
        }

        map[track] = TextView(context)
        map[track]?.textSize = 14f
        map[track]?.text = when (track.type) {
            AlMediaType.TYPE_VIDEO -> "Track ${track.id}"
            AlMediaType.TYPE_AUDIO -> "Track ${track.id}"
            else -> "Unknown Track"
        }
        map[track]?.setBackgroundColor(
            when (track.type) {
                AlMediaType.TYPE_VIDEO -> Color.LTGRAY
                AlMediaType.TYPE_AUDIO -> Color.DKGRAY
                else -> Color.RED
            }
        )
        val padding = applyDimension(TypedValue.COMPLEX_UNIT_DIP, 8f).toInt()
        map[track]?.setPadding(padding, padding, padding, padding)
        addView(map[track])
        requestLayout()
    }

    private fun applyDimension(unit: Int, value: Float): Float {
        val r = if (context == null) {
            Resources.getSystem()
        } else {
            context.resources
        }
        return TypedValue.applyDimension(unit, value, r.displayMetrics)
    }

    override fun onLayout(changed: Boolean, l: Int, t: Int, r: Int, b: Int) {
        super.onLayout(changed, l, t, r, b)
        var height = 0
        map.forEach {
            val track = it.key
            val view = it.value

            var w = view.measuredWidth
            val h = view.measuredHeight
            if (durationInUS > 0) {
                //TODO
            }
            view.layout(l, height, l + w, height + h)
            Log.i(TAG, "layout($l, $height, ${l + w}, ${height + h})")

            height += h
        }
    }

    companion object {
        const val TAG = "AlTrackView"
    }
}