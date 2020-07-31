package com.lmy.samplenative

import android.content.Intent
import android.net.Uri
import android.view.View
import android.widget.Toast
import androidx.recyclerview.widget.DividerItemDecoration
import androidx.recyclerview.widget.RecyclerView
import com.lmy.samplenative.adapter.OnRecyclerItemClickListener
import com.lmy.samplenative.adapter.SimpleTextAdapter
import com.lmy.samplenative.ui.*
import com.microsoft.officeuifabric.listitem.ListItemDivider
import kotlinx.android.synthetic.main.activity_main.*
import java.io.File

class MainActivity : BaseActivity(), OnRecyclerItemClickListener.OnItemClickListener {
    private val ITEMS_MAIN = arrayListOf(
        Item("New Image Editor") { startActivity(Intent(this, AlImageActivity::class.java)) },
        Item("Video Editor V2") { actionVideoV2() },
        Item("Video Recorder") { startActivity(Intent(this, CameraActivity::class.java)) },
        Item("FFmpeg cmd") { startActivity(Intent(this, FFCMDActivity::class.java)) }
    )

    private val ITEMS_MORE = arrayListOf(
        Item("Audio Player") { startActivity(Intent(this, AudioActivity::class.java)) },
        Item("Video Player") { startActivity(Intent(this, VideoActivity::class.java)) },
        Item("Audio Recorder & Mux") {
            startActivity(
                Intent(
                    this,
                    AudioRecordAndMuxActivity::class.java
                )
            )
        },
        Item("Display Recorder") { startActivity(Intent(this, AlDisplayActivity::class.java)) },
        Item("Echo Player") { startActivity(Intent(this, EchoActivity::class.java)) },
        Item("Image Editor") { startActivity(Intent(this, ImageActivity::class.java)) },
        Item("Voice Player") { startActivity(Intent(this, AudioPlayerActivity::class.java)) },
        Item("Voice Recorder") {
            startActivity(
                Intent(
                    this,
                    TestAudioRecorderActivity::class.java
                )
            )
        },
        Item("TestMemFIFOActivity") {
            startActivity(
                Intent(
                    this,
                    TestMemFIFOActivity::class.java
                )
            )
        },
        Item("Show Bitmap") { startActivity(Intent(this, BitmapActivity::class.java)) }
    )
    private val adapter = SimpleTextAdapter()
    private val items = ArrayList<Item>()

    override fun getLayoutResource(): Int = R.layout.activity_main
    override fun initView() {
        setSupportActionBar(toolbar)
        toolbar.title = resources.getString(R.string.app_name)
        toolbar.subtitle = BuildConfig.VERSION_NAME
        SourcesManager(this).initialize()
        adapter.bindData(ArrayList<String>())
        listView.adapter = adapter
        listView.addItemDecoration(ListItemDivider(this, DividerItemDecoration.VERTICAL))
        listView.addOnItemTouchListener(OnRecyclerItemClickListener(this, this))
        showMain()
        if (isNightMode()) {
            rootLayout.setBackgroundColor(resources.getColor(R.color.black_232))
        }
    }

    private fun showMain() {
        items.clear()
        adapter.items.clear()
        ITEMS_MAIN.forEach {
            adapter.items.add(it.name)
            items.add(it)
        }
        adapter.notifyDataSetChanged()
    }

    private fun showMore() {
        items.clear()
        adapter.items.clear()
        ITEMS_MAIN.forEach {
            adapter.items.add(it.name)
            items.add(it)
        }
        ITEMS_MORE.forEach {
            adapter.items.add(it.name)
            items.add(it)
        }
        adapter.notifyDataSetChanged()
    }

    class Item(val name: String, val action: () -> Unit)

    override fun onItemClick(parent: RecyclerView?, view: View?, position: Int) {
        items[position].action()
    }

    private fun actionVideoV2() {
        val testFile = File(externalCacheDir, "/video/hw_small.mp4")
        if (!testFile.exists()) {
            Toast.makeText(this, "File NOT exist.", Toast.LENGTH_SHORT).show()
            return
        }
        val intent = Intent(this, AlVideoV2Activity::class.java)
        intent.data = Uri.fromFile(testFile)
        startActivity(intent)
    }

    fun onClick(v: View) {
        when (v.id) {
            R.id.moreBtn -> {
                showMore()
                v.visibility = View.GONE
            }
        }
    }
}
