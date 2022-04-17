package com.alimin.fk.app.widget

import android.content.Context
import android.util.AttributeSet
import android.view.LayoutInflater
import android.view.View
import android.widget.FrameLayout
import com.alimin.fk.app.R
import com.microsoft.fluentui.theming.FluentUIContextThemeWrapper

class FkYesNoView : FrameLayout {
    private lateinit var cancelBtn: FrameLayout
    private lateinit var enterBtn: FrameLayout

    constructor(context: Context) : super(
        FluentUIContextThemeWrapper(
            context,
            R.style.Theme_FluentUI_ContextualCommandBar
        )
    ) {
        initialize()
    }

    constructor(context: Context, attrs: AttributeSet?)
            : super(
        FluentUIContextThemeWrapper(
            context,
            R.style.Theme_FluentUI_ContextualCommandBar
        ), attrs
    ) {
        initialize()
    }

    private fun initialize() {
        val rootView = LayoutInflater
            .from(context)
            .inflate(R.layout.item_yes_no_action, this, false)
        addView(rootView)
        cancelBtn = rootView.findViewById(R.id.cancelBtn)
        enterBtn = rootView.findViewById(R.id.enterBtn)
        cancelBtn.getChildAt(0).isEnabled = true
        enterBtn.getChildAt(0).isEnabled = true
    }

    fun setActionListener(listener: (yes: Boolean) -> Unit) {
        cancelBtn.setOnClickListener(object : OnClickListener {
            override fun onClick(v: View?) {
                listener(false)
            }
        })
        enterBtn.setOnClickListener(object : OnClickListener {
            override fun onClick(v: View?) {
                listener(true)
            }
        })
    }
}