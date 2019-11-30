package com.lmy.common.adapter

import android.net.Uri
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.BaseAdapter
import android.widget.TextView
import com.facebook.drawee.backends.pipeline.Fresco
import com.facebook.drawee.view.SimpleDraweeView
import com.facebook.imagepipeline.common.ResizeOptions
import com.facebook.imagepipeline.request.ImageRequestBuilder
import com.lmy.common.R
import com.lmy.common.model.entity.Folder
import java.io.File
import java.util.*

/**
 * Created by lmy on 2017/5/18.
 */

class GalleryFolderAdapter : BaseAdapter() {
    private var mWidth = 0
    private var mHeight = 0
    private var items: List<Folder>? = null
    private var mViewHolder: ViewHolder? = null
    var selectIndex = 0
        set(i) {
            if (selectIndex == i) return
            field = i
            notifyDataSetChanged()
        }

    init {
        this.items = ArrayList<Folder>()
    }

    fun bindData(items: List<Folder>) {
        this.items = items
        notifyDataSetChanged()
    }

    override fun getCount(): Int {
        return items!!.size
    }

    override fun getItem(position: Int): Folder {
        return items!![position]
    }

    override fun getItemId(position: Int): Long {
        return position.toLong()
    }

    override fun getView(position: Int, view: View?, parent: ViewGroup): View {
        var view = view
        if (view == null) {
            view = LayoutInflater.from(parent.context).inflate(R.layout.item_gallery_folder, parent, false)
            mViewHolder = ViewHolder(this, view!!)
        } else {
            mViewHolder = view.tag as ViewHolder
        }
        if (mViewHolder != null) {
            mViewHolder!!.onBind(getItem(position), position)
        }
        return view
    }

    private class ViewHolder internal constructor(internal var adapter: GalleryFolderAdapter, itemView: View) {
        internal var imageView: SimpleDraweeView
        internal var textView: TextView
        internal var indicatorView: View

        init {
            imageView = itemView.findViewById(R.id.image)
            textView = itemView.findViewById(R.id.text)
            indicatorView = itemView.findViewById(R.id.indicator)
            itemView.tag = this
        }

        fun onBind(item: Folder, position: Int) {
            if (null == item.cover) {
                return
            }
            textView.text = if (position == 0) textView.resources.getString(R.string.folder_all) else item.name
            indicatorView.visibility = if (adapter.selectIndex == position) View.VISIBLE else View.INVISIBLE
            if (adapter.mWidth == 0) {
                imageView.post(Runnable {
                    adapter.mWidth = imageView.getWidth()
                    adapter.mHeight = imageView.getHeight()
                    load(item.cover!!)
                })
                return
            }
            load(item.cover!!)
        }

        private fun load(cover: File) {
            show(imageView, Uri.fromFile(cover), imageView.getContext(), true)
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
