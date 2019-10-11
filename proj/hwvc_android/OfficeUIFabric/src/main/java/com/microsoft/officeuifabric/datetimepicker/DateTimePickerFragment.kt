/*
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Licensed under the MIT License.
 */

package com.microsoft.officeuifabric.datetimepicker

import android.os.Bundle
import android.support.v4.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import com.microsoft.officeuifabric.R
import com.microsoft.officeuifabric.util.DateTimeUtils
import kotlinx.android.synthetic.main.fragment_date_time_picker.*
import org.threeten.bp.Duration
import org.threeten.bp.ZonedDateTime

/**
 * [DateTimePickerFragment] houses a [DateTimePicker] instance and configures aspects of the view.
 * When in TalkBalk this takes over date picking responsibilities from [DatePickerFragment] and
 * [DateTimePicker] shows months, days and years instead of date, hour, minute and period.
 */
internal class DateTimePickerFragment : Fragment(), OnDateTimeSelectedListener {
    var onDateTimeSelectedListener: OnDateTimeSelectedListener? = null

    val selectedTab: DateTimePicker.Tab
        get() = date_time_picker.selectedTab

    private lateinit var dateRangeMode: DateRangeMode
    private lateinit var pickerMode: DateTimePicker.PickerMode

    fun setDate(date: ZonedDateTime) {
        val range = date_time_picker.timeSlot
        if (DateTimeUtils.isSameDay(date, range.start))
            return
        date_time_picker.timeSlot = TimeSlot(range.start.with(date.toLocalDate()), range.duration)
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val bundle = savedInstanceState ?: arguments ?: return
        dateRangeMode = bundle.getSerializable(DateTimePickerExtras.DATE_RANGE_MODE) as DateRangeMode
    }

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View? {
        return inflater.inflate(R.layout.fragment_date_time_picker, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        val bundle = savedInstanceState ?: arguments
        if (bundle != null) {
            pickerMode = bundle.getSerializable(DateTimePickerExtras.PICKER_MODE) as DateTimePicker.PickerMode

            val dateTime = bundle.getSerializable(DateTimePickerExtras.DATE_TIME) as ZonedDateTime
            val duration = bundle.getSerializable(DateTimePickerExtras.DURATION) as Duration
            date_time_picker.timeSlot = TimeSlot(dateTime, duration)
        }

        date_time_picker.pickerMode = pickerMode
        date_time_picker.onDateTimeSelectedListener = this

        initUI()
    }

    override fun onSaveInstanceState(bundle: Bundle) {
        super.onSaveInstanceState(bundle)

        if (dateRangeMode != DateRangeMode.NONE) {
            dateRangeMode = if (date_time_picker.selectedTab == DateTimePicker.Tab.START)
                DateRangeMode.START
            else
                DateRangeMode.END
        }

        bundle.putSerializable(DateTimePickerExtras.DATE_RANGE_MODE, dateRangeMode)
        bundle.putSerializable(DateTimePickerExtras.DATE_TIME, date_time_picker.timeSlot?.start)
        bundle.putSerializable(DateTimePickerExtras.DURATION, date_time_picker.timeSlot?.duration)
        bundle.putSerializable(DateTimePickerExtras.PICKER_MODE, pickerMode)
    }

    override fun onDateTimeSelected(dateTime: ZonedDateTime, duration: Duration) {
        onDateTimeSelectedListener?.onDateTimeSelected(dateTime, duration)
    }

    private fun initUI() {
        when (dateRangeMode) {
            DateRangeMode.START -> {
                date_time_picker.selectTab(DateTimePicker.Tab.START)
                date_time_picker.setPickerValues(false, false)
            }
            DateRangeMode.END -> {
                date_time_picker.selectTab(DateTimePicker.Tab.END)
                date_time_picker.setPickerValues(true, false)
            }
            DateRangeMode.NONE -> {
                date_time_picker.selectTab(DateTimePicker.Tab.NONE)
                date_time_picker.setPickerValues(false, false)
            }
        }
    }
}