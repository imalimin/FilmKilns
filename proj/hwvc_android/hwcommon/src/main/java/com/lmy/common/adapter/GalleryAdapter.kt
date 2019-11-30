package com.lmy.common.adapter

import android.net.Uri
import android.support.v7.widget.RecyclerView
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.CheckBox
import com.facebook.drawee.backends.pipeline.Fresco

import com.facebook.drawee.view.SimpleDraweeView
import com.facebook.imagepipeline.common.ResizeOptions
import com.facebook.imagepipeline.request.ImageRequestBuilder
import com.lmy.common.R

import java.io.File
import java.util.ArrayList

/**
 * @author limingyi
 * Created on 2017/5/18 0018 18:12.
 */

class GalleryAdapter : RecyclerAdapter<File, GalleryAdapter.ViewHolder>() {
    private var mSelectedItems: MutableList<File>? = null

    val selectedItems: List<File>?
        get() = mSelectedItems

    val selectedCount: Int
        get() = if (mSelectedItems != null) mSelectedItems!!.size else 0

    fun select(recyclerView: RecyclerView, position: Int) {
        if (isSelected(position))
            removeSelectedItem(recyclerView, position)
        else
            addSelectedItem(recyclerView, position)
    }

    private fun addSelectedItem(recyclerView: RecyclerView, position: Int) {
        if (mSelectedItems == null)
            mSelectedItems = ArrayList()
        mSelectedItems!!.add(getItem(position))
        (recyclerView.findViewHolderForAdapterPosition(position) as ViewHolder)
                .select(true)
    }

    private fun removeSelectedItem(recyclerView: RecyclerView, position: Int) {
        if (mSelectedItems == null) return
        mSelectedItems!!.remove(getItem(position))
        (recyclerView.findViewHolderForAdapterPosition(position) as ViewHolder)
                .select(false)
    }

    fun isSelected(position: Int): Boolean {
        if (mSelectedItems == null) return false
        val file = getItem(position)
        for (f in selectedItems!!) {
            if (f === file)
                return true
        }
        return false
    }

    override fun onBindViewHolder(holder: ViewHolder, item: File, position: Int) {
        holder.onBind(item, position)
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        return ViewHolder(this, LayoutInflater.from(parent.context).inflate(R.layout.item_gallery, parent, false))
    }

    class ViewHolder(internal var adapter: GalleryAdapter, itemView: View) : RecyclerAdapter.BaseViewHolder<File>(itemView) {
        internal var imageView: SimpleDraweeView
        internal var checkBox: CheckBox

        init {
            imageView = itemView.findViewById(R.id.image)
            checkBox = itemView.findViewById(R.id.check)
        }

        override fun onBind(item: File, position: Int) {
            checkBox.isChecked = adapter.isSelected(position)
            imageView.post { show(imageView, Uri.fromFile(item), imageView.context, true) }
        }

        fun select(select: Boolean) {
            checkBox.isChecked = select
        }

        private fun show(view: SimpleDraweeView, uri: Uri, context: Any, resize: Boolean) {
            val width = view.width
            val height = view.height
            view.controller = Fresco.newDraweeControllerBuilder().setUri(uri).setCallerContext(context)
                    .setOldController(view.controller)
                    .setImageRequest(if (resize && width > 0 && height > 0) ImageRequestBuilder.newBuilderWithSource(uri)
                            .setResizeOptions(ResizeOptions(width, height)).build() else null)
                    .build()
        }
    }
}
