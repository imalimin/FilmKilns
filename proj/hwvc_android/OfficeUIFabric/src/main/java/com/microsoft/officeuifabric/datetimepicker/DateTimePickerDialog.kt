/*
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Licensed under the MIT License.
 */

package com.microsoft.officeuifabric.datetimepicker

import android.animation.Animator
import android.animation.AnimatorListenerAdapter
import android.app.Dialog
import android.content.Context
import android.os.Bundle
import android.support.v4.app.Fragment
import android.support.v4.app.FragmentManager
import android.support.v4.app.FragmentPagerAdapter
import android.support.v4.content.ContextCompat
import android.support.v4.view.ViewPager
import android.support.v7.widget.Toolbar
import android.view.LayoutInflater
import android.view.MenuItem
import android.view.View
import android.view.ViewGroup
import com.jakewharton.threetenabp.AndroidThreeTen
import android.view.accessibility.AccessibilityEvent

import com.microsoft.officeuifabric.R
import com.microsoft.officeuifabric.calendar.OnDateSelectedListener
import com.microsoft.officeuifabric.datetimepicker.DateTimePicker.PickerMode
import com.microsoft.officeuifabric.util.*
import com.microsoft.officeuifabric.view.ResizableDialog
import kotlinx.android.synthetic.main.dialog_date_time_picker.*
import org.threeten.bp.Duration
import org.threeten.bp.ZonedDateTime

// TODO consider merging PickerMode and DateRangeMode since not all combinations will work
/**
 * [DateTimePickerDialog] provides a dialog view housing both a [DatePickerFragment] and [DateTimePickerFragment] in a [WrapContentViewPager]
 * as well as includes toolbar UI and menu buttons to dismiss the dialog and accept a date/ time
 */
class DateTimePickerDialog : ResizableDialog(), Toolbar.OnMenuItemClickListener, OnDateTimeSelectedListener, OnDateSelectedListener {
    companion object {
        private const val SELECTED_DATE_TIME_TAB = "selectedDateTimeTab"

        @JvmStatic
        fun newInstance(
            context: Context,
            mode: Mode,
            dateRangeMode: DateRangeMode,
            dateTime: ZonedDateTime = getDefaultDateTime(context),
            duration: Duration = Duration.ZERO
        ): DateTimePickerDialog {
            val args = Bundle()
            args.putSerializable(DateTimePickerExtras.DATE_RANGE_MODE, dateRangeMode)
            args.putSerializable(DateTimePickerExtras.DISPLAY_MODE, getDisplayMode(context, dateTime, duration, mode))
            args.putSerializable(DateTimePickerExtras.DATE_TIME, dateTime)
            args.putSerializable(DateTimePickerExtras.DURATION, duration)

            val dialog = DateTimePickerDialog()
            dialog.arguments = args
            return dialog
        }

        private fun getDefaultDateTime(context: Context): ZonedDateTime {
            AndroidThreeTen.init(context)
            return ZonedDateTime.now()
        }

        private fun getDisplayMode(context: Context, dateTime: ZonedDateTime, duration: Duration, mode: Mode): DisplayMode {
            if (context.isAccessibilityEnabled)  {
                return mode.accessibleMode
            } else {
                val endTime = dateTime.plus(duration)
                val isSameDayEvent = DateTimeUtils.isSameDay(dateTime, endTime)
                if (isSameDayEvent || mode == Mode.DATE)
                    return mode.defaultMode
                else
                    return DisplayMode.TIME
            }
        }
    }

    enum class Mode(internal val defaultMode: DisplayMode, internal val accessibleMode: DisplayMode) {
        DATE(DisplayMode.DATE, DisplayMode.ACCESSIBLE_DATE),
        DATE_TIME(DisplayMode.DATE_TIME, DisplayMode.ACCESSIBLE_DATE_TIME),
        TIME_DATE(DisplayMode.TIME_DATE, DisplayMode.ACCESSIBLE_DATE_TIME)
    }

    private enum class DisplayMode(val dateTabIndex: Int, val dateTimeTabIndex: Int) {
        ACCESSIBLE_DATE(0, -1),
        ACCESSIBLE_DATE_TIME(-1, 0),
        DATE(0, -1),
        DATE_TIME(0, 1),
        TIME(-1, 0),
        TIME_DATE(0, 1)
    }

    var onDateTimePickedListener: OnDateTimePickedListener? = null

    // A single point in time or the start time of an event
    private lateinit var dateTime: ZonedDateTime
    private lateinit var duration: Duration
    private lateinit var displayMode: DisplayMode
    private lateinit var dateRangeMode: DateRangeMode
    private lateinit var pagerAdapter: DateTimePagerAdapter
    private var selectedDateTimeTab: DateTimePicker.Tab = DateTimePicker.Tab.START
        get() = pagerAdapter.dateTimePickerFragment?.selectedTab ?: field ?: DateTimePicker.Tab.START

    private val animatorListener = object : AnimatorListenerAdapter() {
        override fun onAnimationCancel(animation: Animator) {
            super.onAnimationCancel(animation)
            pagerAdapter.datePickerFragment?.collapseCalendarView()
        }

        override fun onAnimationEnd(animation: Animator) {
            super.onAnimationEnd(animation)
            pagerAdapter.datePickerFragment?.collapseCalendarView()
        }
    }

    private val pageChangeListener = object : ViewPager.SimpleOnPageChangeListener() {
        override fun onPageSelected(position: Int) {
            updateTitles()
            val datePicker = pagerAdapter.datePickerFragment
            val dateTimePicker = pagerAdapter.dateTimePickerFragment
            if (position == displayMode.dateTabIndex && datePicker != null) {
                view_pager.currentObject = datePicker
                // We're switching from the tall time picker to the short date picker. Layout transition
                // leaves blank white area below date picker. So manual animation is used here instead to avoid this.
                enableLayoutTransition(false)
                view_pager.smoothlyResize(datePicker.fullModeHeight, animatorListener)
            } else if (position == displayMode.dateTimeTabIndex && dateTimePicker != null) {
                view_pager.currentObject = dateTimePicker
                datePicker?.expandCalendarView()
                enableLayoutTransition(true)
                view_pager.shouldWrapContent = true
            }
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val bundle = savedInstanceState ?: arguments ?: return
        dateTime = bundle.getSerializable(DateTimePickerExtras.DATE_TIME) as ZonedDateTime
        duration = bundle.getSerializable(DateTimePickerExtras.DURATION) as Duration
        displayMode = bundle.getSerializable(DateTimePickerExtras.DISPLAY_MODE) as DisplayMode
        dateRangeMode = bundle.getSerializable(DateTimePickerExtras.DATE_RANGE_MODE) as DateRangeMode

        savedInstanceState?.let {
            selectedDateTimeTab = it.getSerializable(SELECTED_DATE_TIME_TAB) as DateTimePicker.Tab
            displayMode = it.getSerializable(DateTimePickerExtras.DISPLAY_MODE) as DisplayMode
        }
    }

    override fun createContentView(inflater: LayoutInflater, parent: ViewGroup?, savedInstanceState: Bundle?): View {
        return inflater.inflate(R.layout.dialog_date_time_picker, parent, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        val context = context ?: return
        toolbar.inflateMenu(R.menu.menu_time_picker)
        toolbar.setOnMenuItemClickListener(this)
        toolbar.navigationIcon = ContextCompat.getDrawable(context, R.drawable.ms_ic_close_grey)
        toolbar.navigationContentDescription = resources.getString(R.string.date_time_picker_accessibility_close_dialog_button)
        toolbar.setNavigationOnClickListener { dismiss() }
        val iconColor = ThemeUtil.getThemeAttrColor(context, R.attr.uifabricDateTimePickerToolbarIconColor)
        toolbar.menu.findItem(R.id.action_done).icon = context.getTintedDrawable(R.drawable.ms_ic_done, iconColor)

        pagerAdapter = DateTimePagerAdapter(childFragmentManager)
        view_pager.adapter = pagerAdapter
        view_pager.addOnPageChangeListener(pageChangeListener)

        if (displayMode == DisplayMode.TIME_DATE)
            view_pager.currentItem = displayMode.dateTimeTabIndex

        if (pagerAdapter.count < 2)
            tab_container.visibility = View.GONE
        else
            tabs.setupWithViewPager(view_pager)

        updateTitles()
    }

    override fun onCreateDialog(savedInstanceState: Bundle?): Dialog {
        return ContentDialog(activity!!, theme)
    }

    override fun onSaveInstanceState(bundle: Bundle) {
        super.onSaveInstanceState(bundle)
        bundle.putSerializable(DateTimePickerExtras.DATE_TIME, dateTime)
        bundle.putSerializable(DateTimePickerExtras.DURATION, duration)
        bundle.putSerializable(DateTimePickerExtras.DISPLAY_MODE, displayMode)
        bundle.putSerializable(DateTimePickerExtras.DATE_RANGE_MODE, dateRangeMode)
        bundle.putSerializable(SELECTED_DATE_TIME_TAB, selectedDateTimeTab)
    }

    override fun onMenuItemClick(item: MenuItem): Boolean {
        (activity as? OnDateTimePickedListener)?.onDateTimePicked(dateTime, duration)
        onDateTimePickedListener?.onDateTimePicked(dateTime, duration)
        dismiss()

        return false
    }

    override fun onDateSelected(dateTime: ZonedDateTime) {
        if (dateRangeMode === DateRangeMode.END) {
            if (dateTime.isBefore(this.dateTime))
                this.dateTime = dateTime.minus(duration)
            else
                duration = dateTime.getNumberOfDaysFrom(this.dateTime)
        } else {
            this.dateTime = this.dateTime.with(dateTime.toLocalDate())
        }

        updateTitles()

        pagerAdapter.dateTimePickerFragment?.setDate(this.dateTime)
    }

    override fun onDateTimeSelected(dateTime: ZonedDateTime, duration: Duration) {
        this.dateTime = dateTime
        this.duration = duration

        updateTitles()

        pagerAdapter.datePickerFragment?.setTimeSlot(TimeSlot(this.dateTime, this.duration))
    }

    private fun updateTitles() {
        val context = context ?: return
        when (displayMode) {
            DisplayMode.DATE -> {
                toolbar.title = if (dateRangeMode == DateRangeMode.START)
                    DateStringUtils.formatDateAbbrevAll(context, dateTime)
                else
                    DateStringUtils.formatDateAbbrevAll(context, dateTime.plus(duration))
            }
            DisplayMode.ACCESSIBLE_DATE -> {
                toolbar.title = if (dateRangeMode != DateRangeMode.NONE)
                    resources.getString(R.string.date_time_picker_choose_date)
                else
                    DateStringUtils.formatMonthDayYear(context, dateTime.plus(duration))
            }
            DisplayMode.TIME, DisplayMode.ACCESSIBLE_DATE_TIME -> {
                toolbar.title = if (dateRangeMode != DateRangeMode.NONE)
                    resources.getString(R.string.date_time_picker_choose_time)
                else
                    DateStringUtils.formatAbbrevDateTime(context, dateTime.plus(duration))
            }
            else -> {
                val currentTab = view_pager.currentItem
                toolbar.setTitle(if (currentTab == displayMode.dateTabIndex) R.string.date_time_picker_choose_date else R.string.date_time_picker_choose_time)

                val tabDate = if (selectedDateTimeTab == DateTimePicker.Tab.END) dateTime.plus(duration) else dateTime

                // Set tab titles
                if (displayMode.dateTabIndex != -1)
                    tabs.getTabAt(displayMode.dateTabIndex)?.text = DateStringUtils.formatDateWithWeekDay(
                        context,
                        if (currentTab == displayMode.dateTabIndex) dateTime else tabDate
                    )

                if (displayMode.dateTimeTabIndex != -1)
                    tabs.getTabAt(displayMode.dateTimeTabIndex)?.text = DateStringUtils.formatAbbrevTime(context, tabDate)
            }
        }
    }

    private inner class DateTimePagerAdapter(fragmentManager: FragmentManager) : FragmentPagerAdapter(fragmentManager) {
        var datePickerFragment: DatePickerFragment? = null
        var dateTimePickerFragment: DateTimePickerFragment? = null

        override fun getItem(position: Int): Fragment {
            val fragment: Fragment = if (useDatePickerFragment(position))
                DatePickerFragment()
            else
                DateTimePickerFragment()

            fragment.arguments = arguments

            return fragment
        }

        override fun getCount(): Int = if ((displayMode == DisplayMode.DATE_TIME || displayMode == DisplayMode.TIME_DATE)) 2 else 1

        override fun instantiateItem(container: ViewGroup, position: Int): Any {
            val fragment = super.instantiateItem(container, position) as Fragment
            val currentItem = view_pager.currentItem
            if (fragment is DatePickerFragment) {
                datePickerFragment = fragment
                datePickerFragment?.arguments?.putBoolean(DateTimePickerExtras.EXPAND_ON_START, position != currentItem)
                datePickerFragment?.onDateSelectedListener = this@DateTimePickerDialog
            } else {
                dateTimePickerFragment = fragment as DateTimePickerFragment
                dateTimePickerFragment?.onDateTimeSelectedListener = this@DateTimePickerDialog

                val pickerMode = if (displayMode == DisplayMode.ACCESSIBLE_DATE) PickerMode.DATE else PickerMode.DATE_TIME
                dateTimePickerFragment?.arguments?.putSerializable(DateTimePickerExtras.PICKER_MODE, pickerMode)
            }

            if (position == currentItem)
                view_pager.currentObject = fragment

            return fragment
        }

        private fun useDatePickerFragment(position: Int): Boolean =
            position == displayMode.dateTabIndex && (displayMode == DisplayMode.DATE || displayMode == DisplayMode.TIME_DATE || displayMode == DisplayMode.DATE_TIME)
    }

    private inner class ContentDialog : Dialog {
        constructor(context: Context, themeResId: Int) : super(context, themeResId)

        override fun dispatchPopulateAccessibilityEvent(event: AccessibilityEvent): Boolean {
            if (event.eventType == AccessibilityEvent.TYPE_WINDOW_STATE_CHANGED) {
                val announcement = when (displayMode) {
                    DisplayMode.DATE, DisplayMode.ACCESSIBLE_DATE -> {
                        if (dateRangeMode != DateRangeMode.NONE)
                            R.string.date_picker_range_accessibility_dialog_title
                        else
                            R.string.date_picker_accessibility_dialog_title
                    }
                    else -> {
                        if (dateRangeMode != DateRangeMode.NONE)
                            R.string.date_time_picker_range_accessibility_dialog_title
                        else
                            R.string.date_time_picker_accessibility_dialog_title
                    }
                }

                event.text.add(resources.getString(announcement))
                return true
            }
            return super.dispatchPopulateAccessibilityEvent(event)
        }
    }
}

interface OnDateTimePickedListener {
    /**
     * Method called when a user picks a date, date and time, or date range start/ end.
     * @param [dateTime] the picked date or date and time
     * @param [duration] the duration of a date range
     */
    fun onDateTimePicked(dateTime: ZonedDateTime, duration: Duration)
}
