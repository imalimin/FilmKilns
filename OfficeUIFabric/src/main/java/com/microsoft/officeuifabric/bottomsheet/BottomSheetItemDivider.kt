/*
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Licensed under the MIT License.
 */

package com.microsoft.officeuifabric.bottomsheet

import android.content.Context
import android.graphics.Canvas
import android.graphics.Rect
import android.view.View
import androidx.recyclerview.widget.RecyclerView
import com.microsoft.officeuifabric.R
import com.microsoft.officeuifabric.listitem.ListItemView
import com.microsoft.officeuifabric.util.ThemeUtil
import com.microsoft.officeuifabric.view.BaseDividerItemDecoration

internal class BottomSheetItemDivider(context: Context) : BaseDividerItemDecoration(context, HORIZONTAL) {
    override fun getItemOffsets(outRect: Rect, view: View, parent: RecyclerView, state: RecyclerView.State) {
        val useDivider = view.getTag(R.id.uifabric_bottom_sheet_item_divider) as? Boolean ?: false
        if (useDivider)
            outRect.set(0, dividerHeight.toInt() + subHeaderDividerPadding.toInt() * 2, 0, 0)
        else
            outRect.setEmpty()
    }

    override fun onDraw(canvas: Canvas, parent: RecyclerView, state: RecyclerView.State) {
        for (index in 0 until parent.childCount) {
            val itemView = parent.getChildAt(index)
            if (itemView is ListItemView) {
                val useDivider = itemView.getTag(R.id.uifabric_bottom_sheet_item_divider) as Boolean
                if (useDivider) {
                    val left =  itemView.left.toFloat()
                    val right = itemView.right.toFloat()
                    drawTopSpacer(canvas, itemView, left, right)
                    drawDivider(
                        canvas,
                        itemView,
                        left,
                        right,
                        true,
                        ThemeUtil.getThemeAttrColor(uifabricContext, R.attr.uifabricBottomSheetDividerColor)
                    )
                    drawBottomSpacer(canvas, itemView, left, right)
                }
            }
        }
    }
}