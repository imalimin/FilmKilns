package com.lmy.samplenative

import android.content.Context
import android.content.Intent
import android.view.View
import android.view.ViewGroup
import android.widget.ArrayAdapter
import android.widget.TextView
import com.lmy.samplenative.ui.TestAudioRecorderActivity
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : BaseActivity() {
    private val ITEMS = arrayListOf(
            Item("Audio Player") { startActivity(Intent(this, AudioPlayerActivity::class.java)) },
            Item("Audio Recorder") { startActivity(Intent(this, TestAudioRecorderActivity::class.java)) },
            Item("TestMemFIFOActivity") { startActivity(Intent(this, TestMemFIFOActivity::class.java)) },
            Item("Echo") { startActivity(Intent(this, EchoActivity::class.java)) },
            Item("Image") { startActivity(Intent(this, ImageActivity::class.java)) },
            Item("Audio") { startActivity(Intent(this, AudioActivity::class.java)) },
            Item("Video") { startActivity(Intent(this, VideoActivity::class.java)) }
    )

    override fun getLayoutResource(): Int = R.layout.activity_main
    override fun initView() {
        listView.adapter = Adapter(this, R.layout.item_simple_text, ITEMS)
        listView.setOnItemClickListener { parent, view, position, id ->
            (listView.adapter as Adapter).getItem(position).action()
        }
    }

    class Item(val name: String, val action: () -> Unit)

    class Adapter(context: Context, resource: Int, items: List<Item>)
        : ArrayAdapter<Item>(context, resource, items) {
        override fun getView(position: Int, convertView: View?, parent: ViewGroup?): View {
            val itemView = convertView ?: super.getView(position, convertView, parent)
            val textView = itemView.findViewById<TextView>(R.id.text1)
            textView?.text = getItem(position).name
            return itemView
        }
    }
}
