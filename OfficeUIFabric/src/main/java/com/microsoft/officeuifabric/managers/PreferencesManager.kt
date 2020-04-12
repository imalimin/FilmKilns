/*
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Licensed under the MIT License.
 */

package com.microsoft.officeuifabric.managers

import android.content.Context
import org.threeten.bp.DayOfWeek
import org.threeten.bp.temporal.WeekFields
import java.util.Locale

/**
 * [PreferencesManager] helper methods dealing with device SharedPreferences
 */
object PreferencesManager {
    private const val PREF_NAME = "prefs"
    private const val PREF_KEY_WEEK_START = "weekStart"

    /**
     * Sets the local first day of the week
     */
    @JvmStatic
    fun setWeekStart(context: Context, weekStart: DayOfWeek) {
        val sharedPreferences = context.getSharedPreferences(PREF_NAME, Context.MODE_PRIVATE)
        sharedPreferences.edit().putInt(PREF_KEY_WEEK_START, weekStart.value).apply()
    }

    /**
     * Gets the local first day of the week
     */
    @JvmStatic
    fun getWeekStart(context: Context): DayOfWeek {
        val defaultWeekStart = WeekFields.of(Locale.getDefault()).firstDayOfWeek
        val sharedPreferences = context.getSharedPreferences(PREF_NAME, Context.MODE_PRIVATE)
        val weekStart = sharedPreferences.getInt(PREF_KEY_WEEK_START, defaultWeekStart.value)
        return DayOfWeek.of(weekStart)
    }
}
