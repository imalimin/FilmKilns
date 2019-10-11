/*
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Licensed under the MIT License.
 */

package com.microsoft.officeuifabric.calendar

import android.content.Context
import android.content.res.ColorStateList
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.graphics.PorterDuff
import android.graphics.drawable.ColorDrawable
import android.graphics.drawable.Drawable
import android.graphics.drawable.StateListDrawable
import android.os.Build
import android.support.v4.content.ContextCompat
import android.support.v4.view.ViewCompat
import android.support.v4.widget.TextViewCompat
import android.support.v7.widget.AppCompatButton
import android.text.SpannableStringBuilder
import android.text.Spanned
import android.text.style.AbsoluteSizeSpan
import android.util.AttributeSet
import android.view.Gravity
import android.view.View
import android.view.accessibility.AccessibilityEvent
import android.view.accessibility.AccessibilityNodeInfo
import android.widget.Checkable
import com.microsoft.officeuifabric.R
import com.microsoft.officeuifabric.util.DateStringUtils
import com.microsoft.officeuifabric.util.DateTimeUtils
import com.microsoft.officeuifabric.util.ThemeUtil
import com.microsoft.officeuifabric.util.isAccessibilityEnabled
import org.threeten.bp.LocalDate
import org.threeten.bp.ZonedDateTime
import org.threeten.bp.format.DateTimeFormatter
import org.threeten.bp.temporal.ChronoUnit
import java.util.*

// TODO add ripple for Lollipop

/**
 * [CalendarDayView] View that displays a day of the week
 */
internal class CalendarDayView: AppCompatButton, Checkable {
    /**
     * sets the date of the View
     */
    var date: LocalDate = LocalDate.now()
        set(value) {
            field = value
            val today = LocalDate.now()
            updateBackgroundColor(today)
            updateText()
            setTextColor(textDayColor)
            updateContentDescription()
            isActivated = DateTimeUtils.isSameDay(today, field)

            ViewCompat.postInvalidateOnAnimation(this)
        }

    /**
     * sets the selected drawable to use
     */
    var selectedDrawable: Drawable? = null
        set(value) {
            field = value
            field?.setBounds(0, 0, measuredWidth, measuredHeight)
            field?.setColorFilter(config.selectionAccentColor, PorterDuff.Mode.SRC_ATOP)

            ViewCompat.postInvalidateOnAnimation(this)
        }

    // underscore prevents JVM platform declaration clash
    private var _foregroundDrawable: Drawable? = null
        set(value) {
            if (field == value)
                return

            // reset foreground drawable
            field?.callback = null
            unscheduleDrawable(field)

            // set foreground drawable
            field = value
            field?.let {
                it.callback = this
                it.state = drawableState
            }

            ViewCompat.postInvalidateOnAnimation(this)
        }

    private lateinit var config: CalendarView.Config

    private var paint = Paint()

    private var _isChecked = false

    private var regularAppearance = 0
    private var todayAppearance = 0
    private var checkedAppearance = 0

    private var todayBackgroundDrawable: Drawable? = null

    private var textDayColor: ColorStateList? = null
    private var viewBackgroundColor = Color.TRANSPARENT

    /**
     * @param [context] Context
     * @param [calendarConfig] Config passes CalendarView attributes
     * @constructor creates an instance of a CalendarDayView
     */
    constructor(context: Context, calendarConfig: CalendarView.Config) : super(context) {
        config = calendarConfig
        setWillNotDraw(false)

        todayBackgroundDrawable = ContextCompat.getDrawable(context, R.drawable.calendar_background_today)

        paint.isAntiAlias = true

        regularAppearance = R.style.TextAppearance_UIFabric_CalendarDay
        todayAppearance = R.style.TextAppearance_UIFabric_CalendarDay2
        checkedAppearance = R.style.TextAppearance_UIFabric_CalendarDay2

        textDayColor = config.calendarDayTextColor

        background = null
        gravity = Gravity.CENTER
        includeFontPadding = false
        TextViewCompat.setTextAppearance(this, regularAppearance)
        isAllCaps = false

        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.M)
            _foregroundDrawable = createBackgroundStateList()

        setPadding(0, 0, 0, 0)
    }

    @JvmOverloads
    constructor(context: Context, attrs: AttributeSet? = null, defStyleAttr: Int = 0) : super(context, attrs, defStyleAttr)

    override fun setChecked(checked: Boolean) {
        if (_isChecked == checked)
            return

        _isChecked = checked
        updateText()
        updateTypeface()
        refreshDrawableState()

        if (context.isAccessibilityEnabled) {
            sendAccessibilityEvent(AccessibilityEvent.CONTENT_CHANGE_TYPE_UNDEFINED)
        }

        ViewCompat.postInvalidateOnAnimation(this)
    }

    override fun isChecked(): Boolean {
        return _isChecked
    }

    override fun toggle() {
        _isChecked = !_isChecked
    }

    override fun setActivated(activated: Boolean) {
        val wasActivated = isActivated
        super.setActivated(activated)
        if (wasActivated == activated)
            return

        updateText()
        setTextColor(textDayColor)
        updateTypeface()

        ViewCompat.postInvalidateOnAnimation(this)
    }

    override fun setOnClickListener(listener: View.OnClickListener?) {
        super.setOnClickListener(listener)
        isClickable = listener != null
    }

    override fun onMeasure(widthMeasureSpec: Int, heightMeasureSpec: Int) {
        super.onMeasure(widthMeasureSpec, widthMeasureSpec)
        todayBackgroundDrawable?.setBounds(0, 0, measuredWidth, measuredHeight)
        selectedDrawable?.setBounds(0, 0, measuredWidth, measuredHeight)
        _foregroundDrawable?.setBounds(0, 0, measuredWidth, measuredHeight)
    }

    override fun onDraw(canvas: Canvas) {
        val width = measuredWidth
        val height = measuredHeight

        if (viewBackgroundColor != Color.TRANSPARENT) {
            paint.color = viewBackgroundColor
            canvas.drawRect(0f, 0f, width.toFloat(), height.toFloat(), paint)
        }

        if (isChecked && selectedDrawable != null) {
            selectedDrawable?.setBounds(0, 0, measuredWidth, measuredHeight)
            selectedDrawable?.draw(canvas)
        } else if (isActivated) {
            todayBackgroundDrawable?.draw(canvas)
        }

        _foregroundDrawable?.draw(canvas)

        super.onDraw(canvas)
    }

    override fun onCreateDrawableState(extraSpace: Int): IntArray {
        val drawableState = super.onCreateDrawableState(extraSpace + 1)
        if (isChecked) {
            View.mergeDrawableStates(drawableState, intArrayOf(android.R.attr.state_checked))
        }
        return drawableState
    }

    override fun verifyDrawable(who: Drawable): Boolean {
        return super.verifyDrawable(who) || who === _foregroundDrawable
    }

    override fun jumpDrawablesToCurrentState() {
        super.jumpDrawablesToCurrentState()
        _foregroundDrawable?.let { if (it.isStateful) it.jumpToCurrentState() }
    }

    override fun drawableStateChanged() {
        super.drawableStateChanged()
        _foregroundDrawable?.let { if (it.isStateful) it.state = drawableState }
    }

    override fun drawableHotspotChanged(x: Float, y: Float) {
        super.drawableHotspotChanged(x, y)
        _foregroundDrawable?.setHotspot(x, y)
    }

    override fun onInitializeAccessibilityNodeInfo(info: AccessibilityNodeInfo) {
        updateContentDescription()
        super.onInitializeAccessibilityNodeInfo(info)
    }

    private fun updateText() {
        val dayOfMonth = date.dayOfMonth
        if (dayOfMonth == 1 && !isChecked) {
            val stringBuilder = SpannableStringBuilder()

            stringBuilder.append(DateTimeFormatter.ofPattern("MMM").format(date))
            stringBuilder.setSpan(
                AbsoluteSizeSpan(config.calendarDayMonthYearTextSize),
                0,
                stringBuilder.length,
                Spanned.SPAN_EXCLUSIVE_EXCLUSIVE
            )

            stringBuilder.append("\n")
            stringBuilder.append(Integer.toString(date.dayOfMonth))

            if (date.year != ZonedDateTime.now().year) {
                stringBuilder.append("\n")
                val idx = stringBuilder.length
                stringBuilder.append(Integer.toString(date.year))
                stringBuilder.setSpan(
                    AbsoluteSizeSpan(config.calendarDayMonthYearTextSize),
                    idx,
                    stringBuilder.length,
                    Spanned.SPAN_EXCLUSIVE_EXCLUSIVE
                )
            }

            text = stringBuilder
        } else {
            text = String.format(Locale.ROOT, Integer.toString(dayOfMonth))
        }
    }

    private fun updateBackgroundColor(localDate: LocalDate) {
        val numMonths = Math.abs(ChronoUnit.MONTHS.between(date.withDayOfMonth(1), localDate.withDayOfMonth(1)))
        viewBackgroundColor =
            if (config.differentiateOddEvenMonth) {
                if (numMonths % 2 == 0L) Color.TRANSPARENT else config.otherMonthBackgroundColor
            } else {
                if (date.isBefore(LocalDate.now())) config.otherMonthBackgroundColor else Color.TRANSPARENT
            }
    }

    private fun updateContentDescription() {
        val formattedDate = DateStringUtils.formatDateWithWeekDay(context, date)
        val stringBuilder = StringBuilder(formattedDate)

        if (isActivated) {
            stringBuilder.append(", ")
            stringBuilder.append(resources.getString(R.string.accessibility_today))
        }

        if (isChecked) {
            stringBuilder.append(", ")
            stringBuilder.append(resources.getString(R.string.accessibility_selected))
        }

        contentDescription = stringBuilder.toString()
    }

    private fun updateTypeface() {
        val appearance = when {
            isActivated -> todayAppearance
            isChecked -> checkedAppearance
            else -> regularAppearance
        }
        TextViewCompat.setTextAppearance(this, appearance)
    }

    // Create this in code instead of xml to support Lollipop, which does not allow attributes in xml selectors.
    private fun createBackgroundStateList(): StateListDrawable {
        val colorDrawable = ColorDrawable(ThemeUtil.getThemeAttrColor(context, R.attr.uifabricBackgroundPressedColor))
        val stateListDrawable = StateListDrawable()
        stateListDrawable.addState(
            intArrayOf(android.R.attr.state_focused, android.R.attr.state_pressed),
            colorDrawable
        )
        stateListDrawable.addState(
            intArrayOf(-android.R.attr.state_focused, android.R.attr.state_pressed),
            colorDrawable
        )
        stateListDrawable.addState(
            intArrayOf(android.R.attr.state_focused),
            colorDrawable
        )
        stateListDrawable.addState(
            intArrayOf(android.R.attr.state_selected),
            colorDrawable
        )
        stateListDrawable.addState(
            intArrayOf(),
            ColorDrawable(viewBackgroundColor)
        )
        return stateListDrawable
    }
}