/*
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Licensed under the MIT License.
 */

package com.microsoft.officeuifabric.view

import android.content.Context
import android.graphics.Canvas
import android.graphics.Paint
import android.view.View
import androidx.annotation.ColorInt
import androidx.core.content.ContextCompat
import androidx.recyclerview.widget.DividerItemDecoration
import com.microsoft.officeuifabric.R
import com.microsoft.officeuifabric.theming.UIFabricContextThemeWrapper
import com.microsoft.officeuifabric.util.ThemeUtil

open class BaseDividerItemDecoration : DividerItemDecoration {
    protected var uifabricContext: Context
    protected val dividerHeight: Float
    protected val subHeaderDividerPadding: Float

    private val dividerPaint: Paint = Paint()
    private val spacerPaint: Paint = Paint()

    constructor(context: Context, orientation: Int) : super(UIFabricContextThemeWrapper(context), orientation) {
        uifabricContext = UIFabricContextThemeWrapper(context)

        dividerHeight = context.resources.getDimension(R.dimen.uifabric_divider_height)
        subHeaderDividerPadding = context.resources.getDimension(R.dimen.uifabric_list_sub_header_divider_padding)

        dividerPaint.style = Paint.Style.FILL

        spacerPaint.style = Paint.Style.FILL
        spacerPaint.color = ContextCompat.getColor(context, android.R.color.transparent)
    }

    protected fun drawTopSpacer(canvas: Canvas, itemView: View, left: Float, right: Float) {
        canvas.drawRect(
            left,
            getTopOfTopSpacer(itemView),
            right,
            getBottomOfTopSpacer(itemView),
            spacerPaint
        )
    }

    protected fun drawDivider(
        canvas: Canvas,
        itemView: View,
        left: Float,
        right: Float,
        useSectionDivider: Boolean,
        @ColorInt color: Int = ThemeUtil.getThemeAttrColor(uifabricContext, R.attr.uifabricDividerColor)
    ) {
        dividerPaint.color = color
        canvas.drawRect(
            left,
            getTopOfDivider(itemView, useSectionDivider),
            right,
            getBottomOfDivider(itemView, useSectionDivider),
            dividerPaint
        )
    }

    protected fun drawBottomSpacer(canvas: Canvas, itemView: View, left: Float, right: Float) {
        canvas.drawRect(
            left,
            getTopOfBottomSpacer(itemView),
            right,
            getBottomOfBottomSpacer(itemView),
            spacerPaint
        )
    }

    private fun getTopOfTopSpacer(itemView: View): Float {
        return itemView.top.toFloat() - (subHeaderDividerPadding * 2) - dividerHeight
    }

    private fun getBottomOfTopSpacer(itemView: View): Float {
        return getTopOfTopSpacer(itemView) + subHeaderDividerPadding
    }

    private fun getTopOfDivider(itemView: View, useSectionDivider: Boolean): Float {
        return if (useSectionDivider) getBottomOfTopSpacer(itemView) else itemView.top.toFloat() - dividerHeight
    }

    private fun getBottomOfDivider(itemView: View, useSectionDivider: Boolean): Float {
        return getTopOfDivider(itemView, useSectionDivider) + dividerHeight
    }

    private fun getTopOfBottomSpacer(itemView: View): Float {
        return getBottomOfDivider(itemView, true)
    }

    private fun getBottomOfBottomSpacer(itemView: View): Float {
        return getTopOfBottomSpacer(itemView) + subHeaderDividerPadding
    }
}