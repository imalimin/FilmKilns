/*
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Licensed under the MIT License.
 */

package com.microsoft.officeuifabric.view

import android.content.Context
import android.graphics.Canvas
import android.graphics.Paint
import android.support.v4.content.ContextCompat
import android.support.v7.widget.DividerItemDecoration
import android.view.View
import com.microsoft.officeuifabric.R
import com.microsoft.officeuifabric.util.ThemeUtil

open class BaseDividerItemDecoration : DividerItemDecoration {
    protected val dividerHeight: Float
    protected val subHeaderDividerPadding: Float

    private val dividerPaint: Paint = Paint()
    private val spacerPaint: Paint = Paint()

    constructor(context: Context, orientation: Int) : super(context, orientation) {
        dividerHeight = context.resources.getDimension(R.dimen.uifabric_divider_height)
        subHeaderDividerPadding = context.resources.getDimension(R.dimen.uifabric_list_sub_header_divider_padding)

        dividerPaint.style = Paint.Style.FILL
        dividerPaint.color = ThemeUtil.getThemeAttrColor(context, R.attr.uifabricDividerColor)

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

    protected fun drawDivider(canvas: Canvas, itemView: View, left: Float, right: Float, useSectionDivider: Boolean) {
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