package com.alimin.pic.widget

import android.content.Context
import android.util.AttributeSet
import android.view.LayoutInflater
import android.view.View
import android.widget.FrameLayout
import com.alimin.pic.R
import com.microsoft.fluentui.contextualcommandbar.CommandItem
import com.microsoft.fluentui.theming.FluentUIContextThemeWrapper

class FkYesNoView : FrameLayout {
    private lateinit var baseCommandBar: FkContextualCommandBar

    constructor(context: Context) : super(
        FluentUIContextThemeWrapper(
            context,
            com.microsoft.fluentui.ccb.R.style.Theme_FluentUI_ContextualCommandBar
        )
    ) {
        initialize()
    }

    constructor(context: Context, attrs: AttributeSet?)
            : super(
        FluentUIContextThemeWrapper(
            context,
            com.microsoft.fluentui.ccb.R.style.Theme_FluentUI_ContextualCommandBar
        ), attrs
    ) {
        initialize()
    }

    private fun initialize() {
        val rootView = LayoutInflater
            .from(context)
            .inflate(R.layout.item_yes_no_action, this, false)
        addView(rootView)
        baseCommandBar = rootView.findViewById(R.id.baseCommandBar)
        with(baseCommandBar) {
            setMenu(R.menu.menu_yes_no)
        }
    }

    fun setActionListener(listener: (yes: Boolean) -> Unit) {
        baseCommandBar.setItemOnClickListener(object : CommandItem.OnItemClickListener {
            override fun onItemClick(item: CommandItem, view: View) {
                when (item.getId()) {
                    R.id.action_cancel -> {
                        listener(false)
                    }
                    R.id.action_enter -> {
                        listener(true)
                    }
                }
            }
        })
    }
}