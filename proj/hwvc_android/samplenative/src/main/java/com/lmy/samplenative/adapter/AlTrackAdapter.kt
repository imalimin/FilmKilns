package com.lmy.samplenative.adapter

import android.graphics.Color
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import com.lmy.samplenative.R
import com.lmy.hwvcnative.entity.AlMediaTrack
import com.lmy.hwvcnative.entity.AlMediaType

class AlTrackAdapter :
    RecyclerAdapter<AlMediaTrack, RecyclerAdapter.BaseViewHolder<AlMediaTrack>>() {

    override fun onBindViewHolder(
        holder: BaseViewHolder<AlMediaTrack>?,
        item: AlMediaTrack?,
        position: Int
    ) {
        holder?.onBind(item, position)
    }

    override fun onCreateViewHolder(
        parent: ViewGroup,
        viewType: Int
    ): BaseViewHolder<AlMediaTrack> = ViewHolder(
        LayoutInflater.from(parent.context).inflate(R.layout.item_media_track, parent, false)
    )

    inner class ViewHolder(itemView: View) :
        RecyclerAdapter.BaseViewHolder<AlMediaTrack>(itemView) {
        private val root: View = itemView.findViewById(R.id.layout)
        private val textView: TextView = itemView.findViewById(R.id.textView)
        override fun onBind(item: AlMediaTrack, position: Int) {
            textView.text = when (item.type) {
                AlMediaType.TYPE_VIDEO -> "Track ${item.id}"
                AlMediaType.TYPE_AUDIO -> "Track ${item.id}"
                else -> "Unknown Track"
            }
            root.setBackgroundColor(when (item.type) {
                AlMediaType.TYPE_VIDEO -> root.resources.getColor(R.color.uifabric_gray_600)
                AlMediaType.TYPE_AUDIO -> root.resources.getColor(R.color.uifabric_gray_700)
                else -> Color.RED
            })
        }
    }
}