/*
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Licensed under the MIT License.
 */

package com.microsoft.officeuifabric.bottomsheet

import android.content.Context
import android.view.ContextThemeWrapper
import android.view.LayoutInflater
import android.view.ViewGroup
import androidx.annotation.StyleRes
import androidx.recyclerview.widget.RecyclerView
import com.microsoft.officeuifabric.R
import com.microsoft.officeuifabric.listitem.ListItemView
import com.microsoft.officeuifabric.util.createImageView

internal class BottomSheetAdapter : RecyclerView.Adapter<RecyclerView.ViewHolder> {
    var onBottomSheetItemClickListener: BottomSheetItem.OnClickListener? = null

    private val context: Context
    private val items: ArrayList<BottomSheetItem>
    private val themeId: Int

    constructor(context: Context, items: ArrayList<BottomSheetItem>, @StyleRes themeId: Int) {
        this.context = context
        this.items = items
        this.themeId = themeId
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): RecyclerView.ViewHolder {
        var inflater = LayoutInflater.from(parent.context)
        if (themeId != 0) {
            val contextThemeWrapper = ContextThemeWrapper(parent.context, themeId)
            inflater = inflater.cloneInContext(contextThemeWrapper)
        }

        val itemView = inflater.inflate(R.layout.view_bottom_sheet_item, parent, false)
        return BottomSheetItemViewHolder(itemView as ListItemView)
    }

    override fun onBindViewHolder(holder: RecyclerView.ViewHolder, position: Int) {
        (holder as? BottomSheetItemViewHolder)?.setBottomSheetItem(items[position])
    }

    override fun getItemCount(): Int = items.size

    private inner class BottomSheetItemViewHolder : RecyclerView.ViewHolder {
        private val listItemView: ListItemView

        constructor(itemView: ListItemView) : super(itemView) {
            listItemView = itemView
        }

        fun setBottomSheetItem(item: BottomSheetItem) {
            listItemView.customView = context.createImageView(item.imageId, item.getImageTint(context))
            listItemView.title = item.title
            listItemView.subtitle = item.subtitle
            listItemView.setTag(R.id.uifabric_bottom_sheet_item_divider, item.useDivider)
            listItemView.layoutDensity = ListItemView.LayoutDensity.COMPACT
            listItemView.background = R.drawable.bottom_sheet_item_ripple_background

            listItemView.setOnClickListener {
                onBottomSheetItemClickListener?.onBottomSheetItemClick(item)
            }
        }
    }
}