/*
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Licensed under the MIT License.
 */

package com.microsoft.officeuifabric.view

import android.animation.LayoutTransition
import android.content.Context
import android.graphics.Rect
import android.os.Bundle
import android.support.v4.app.DialogFragment
import android.view.LayoutInflater
import android.view.MotionEvent
import android.view.View
import android.view.ViewGroup
import android.view.WindowManager
import kotlinx.android.synthetic.main.dialog_resizable.*

import com.microsoft.officeuifabric.R
import com.microsoft.officeuifabric.util.isTablet

/**
 * [ResizableDialog] is a dialog that animates on resize. This enables a transition between views
 * housed in the same dialog
 */
abstract class ResizableDialog : DialogFragment() {

    private val Context.getDesiredDialogSize: IntArray
        get() {
            val metrics = resources.displayMetrics
            val displaySize = Rect()
            displaySize.set(0, 0, metrics.widthPixels, metrics.heightPixels)
            val dialogSize = IntArray(2)
            if (isTablet) {
                dialogSize[0] = WindowManager.LayoutParams.WRAP_CONTENT
            } else {
                dialogSize[0] = metrics.widthPixels
            }
            dialogSize[1] = WindowManager.LayoutParams.WRAP_CONTENT

            return dialogSize
        }

    private val layoutTransition = LayoutTransition()

    fun enableLayoutTransition(enabled: Boolean) {
        (view as ViewGroup).layoutTransition = if (enabled) layoutTransition else null
    }

    override fun onCreateView(inflater: LayoutInflater, parent: ViewGroup?, savedInstanceState: Bundle?): View? {
        return inflater.inflate(R.layout.dialog_resizable, parent, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        val contentView = createContentView(LayoutInflater.from(context), view as ViewGroup, savedInstanceState)
        card_view_container.addView(contentView)

        // Resize animation
        layoutTransition.enableTransitionType(LayoutTransition.CHANGING)
        view.layoutTransition = layoutTransition

        // Dismiss dialog after tapping on spare area
        view.setOnTouchListener(object : View.OnTouchListener {
            private val rect = Rect()

            override fun onTouch(v: View, event: MotionEvent): Boolean {
                if (isCancelable) {
                    card_view_container.getHitRect(rect)
                    if (!rect.contains(event.x.toInt(), event.y.toInt())) {
                        dismiss()
                        return true
                    }
                }

                return false
            }
        })
    }

    override fun onStart() {
        super.onStart()

        val window = dialog?.window ?: return
        activity?.let {
            val dialogSize = it.getDesiredDialogSize
            window.setLayout(dialogSize[0], WindowManager.LayoutParams.MATCH_PARENT)
        }
    }

    override fun getTheme(): Int = R.style.Dialog_UIFabric

    protected abstract fun createContentView(inflater: LayoutInflater, parent: ViewGroup?, savedInstanceState: Bundle?): View
}
