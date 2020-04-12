package com.lmy.file.adapter

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import com.lmy.common.adapter.RecyclerAdapter
import com.lmy.common.ext.lastModifiedTime
import com.lmy.common.utils.debug_i
import com.lmy.common.widget.RoundProgressView
import com.lmy.file.Discoverer
import com.lmy.file.R
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.launch
import java.io.File

class FileAdapter(private val discoverer: () -> Discoverer) : RecyclerAdapter<String, RecyclerAdapter.BaseViewHolder<String>>() {
    private val job = Job()
    private val uiScope = CoroutineScope(Dispatchers.Main + job)

    override fun onBindViewHolder(holder: RecyclerAdapter.BaseViewHolder<String>, item: String, position: Int) {
        holder.onBind(item, position)
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): RecyclerAdapter.BaseViewHolder<String> {
        return ViewHolder(LayoutInflater.from(parent.context)
                .inflate(R.layout.item_file, parent, false), uiScope, discoverer)
    }

    class ViewHolder(itemView: View, private val uiScope: CoroutineScope, private val discoverer: () -> Discoverer)
        : RecyclerAdapter.BaseViewHolder<String>(itemView) {
        private val iconView: TextView = itemView.findViewById(R.id.iconView)
        private val nameView: TextView = itemView.findViewById(R.id.nameView)
        private val sizeView: TextView = itemView.findViewById(R.id.sizeView)
        private val progressView: RoundProgressView = itemView.findViewById(R.id.progressView)
        private val notify = { progress: Float, index: Int, curName: String ->
            uiScope.launch {
                progressView.visibility = if (progress < 1f) View.VISIBLE else View.GONE
                progressView.setProgress(progress)
                debug_i("$curName: progress=$progress, index=$index")
            }
            Unit
        }

        override fun onBind(item: String, position: Int) {
            val path = "${discoverer().getCurrentPath()}/$item"
            if (null != itemView.tag) {
                discoverer().unwatch(itemView.tag as String, notify)
            }
            discoverer().watch(path, notify)
            progressView.visibility = View.GONE
            itemView.tag = path
            nameView.text = item
            val file = File(path)
            sizeView.text = file.lastModifiedTime()
//                    if (file.exists() && file.isDirectory) {
//                "256KB"
//            } else if (file.exists() && file.isFile) {
//                file.size(itemView.context)
//            } else {
//                ""
//            }
            iconView.text = if (discoverer().isDirectory(item)) {
                "D"
            } else {
                "F"
            }
        }
    }
}