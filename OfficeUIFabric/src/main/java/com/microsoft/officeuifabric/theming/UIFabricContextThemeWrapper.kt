/*
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Licensed under the MIT License.
 */

package com.microsoft.officeuifabric.theming

import android.content.Context
import android.content.res.Resources
import androidx.appcompat.view.ContextThemeWrapper
import com.microsoft.officeuifabric.R

class UIFabricContextThemeWrapper(base: Context) : ContextThemeWrapper(base, R.style.Theme_UIFabric) {
    override fun onApplyThemeResource(theme: Resources.Theme, resid: Int, first: Boolean) {
        // We don't want to force our styles on top of the user's. We want their styles to take precedence.
        theme.applyStyle(resid, false)
    }
}