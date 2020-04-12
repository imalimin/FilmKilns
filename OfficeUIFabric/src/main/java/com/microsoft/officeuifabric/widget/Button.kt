/*
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Licensed under the MIT License.
 */

package com.microsoft.officeuifabric.widget

import android.content.Context
import androidx.appcompat.widget.AppCompatButton
import android.util.AttributeSet
import com.microsoft.officeuifabric.R
import com.microsoft.officeuifabric.theming.UIFabricContextThemeWrapper

/**
 * Use this [Button] to get access to the UIFabric theme attribute default values via [UIFabricContextThemeWrapper]
 * without needing to extend Theme.UIFabric in your app's theme.
 */
class Button : AppCompatButton {
    @JvmOverloads
    constructor(context: Context, attrs: AttributeSet? = null, defStyleAttr: Int = R.attr.buttonStyle)
        : super(UIFabricContextThemeWrapper(context), attrs, defStyleAttr)
}