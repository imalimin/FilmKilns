/*
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Licensed under the MIT License.
 */

package com.microsoft.officeuifabric.widget

import android.content.Context
import android.util.AttributeSet
import android.widget.ProgressBar
import com.microsoft.officeuifabric.theming.UIFabricContextThemeWrapper

/**
 * Use this [ProgressBar] to get access to the UIFabric theme attribute default values via [UIFabricContextThemeWrapper]
 * without needing to extend Theme.UIFabric in your app's theme.
 */
class ProgressBar : ProgressBar {
    @JvmOverloads
    constructor(context: Context, attrs: AttributeSet? = null, defStyleAttr: Int = 0, defStyleRes: Int = 0)
        : super(UIFabricContextThemeWrapper(context), attrs, defStyleAttr, defStyleRes)
}