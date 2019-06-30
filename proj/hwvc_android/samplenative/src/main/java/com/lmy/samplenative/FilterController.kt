package com.lmy.samplenative

import android.content.Context
import android.os.Environment
import android.support.v7.app.AlertDialog
import android.support.v7.widget.LinearLayoutManager
import android.support.v7.widget.RecyclerView
import android.util.Log
import android.view.*
import android.widget.SeekBar
import android.widget.TextView
import com.lmy.hwvcnative.FilterSupport
import com.lmy.hwvcnative.filter.*
import com.lmy.samplenative.adapter.OnRecyclerItemClickListener
import com.lmy.samplenative.adapter.RecyclerAdapter
import java.io.File
import java.io.FileInputStream
import java.io.FileOutputStream
import java.io.InputStream
import java.lang.Exception
import java.util.*

/**
 * Created by lmyooyo@gmail.com on 2018/7/24.
 */
class FilterController(private val filterSupport: FilterSupport,
                       private val progressLayout: ViewGroup)
    : SeekBar.OnSeekBarChangeListener, OnRecyclerItemClickListener.OnItemClickListener {

    companion object {
        private val FILTERS = arrayOf(
                "Normal", "Beauty V4", "Beach", "Clean", "Pink")
    }

    private var oneBar: SeekBar = progressLayout.getChildAt(0) as SeekBar
    private var twoBar: SeekBar = progressLayout.getChildAt(1) as SeekBar
    private var thBar: SeekBar = progressLayout.getChildAt(2) as SeekBar
    private var fBar: SeekBar = progressLayout.getChildAt(3) as SeekBar
    private var dialog: AlertDialog? = null
    private lateinit var hwfDir: String

    init {
        oneBar.setOnSeekBarChangeListener(this)
        twoBar.setOnSeekBarChangeListener(this)
        thBar.setOnSeekBarChangeListener(this)
        fBar.setOnSeekBarChangeListener(this)
        initResources()
    }

    private fun createView(): View {
        hwfDir = "${progressLayout.context.externalCacheDir.path}/hwf"
        val layout = LayoutInflater.from(progressLayout.context).inflate(R.layout.dialog_filter, null)
        val recyclerView = layout.findViewById<RecyclerView>(R.id.recyclerView)
        recyclerView.layoutManager = LinearLayoutManager(recyclerView.context,
                LinearLayoutManager.HORIZONTAL, false)
        recyclerView.adapter = Adapter().apply {
            bindData(FilterItem.from(FILTERS))
        }
        recyclerView.addOnItemTouchListener(OnRecyclerItemClickListener(progressLayout.context, this))
        return layout
    }

    fun chooseFilter(context: Context) {
        if (null != dialog && dialog!!.isShowing) dialog?.dismiss()
        dialog = AlertDialog.Builder(context, R.style.BaseAlertDialog_Bottom_Wide)
                .setView(createView())
                .create()
        dialog?.window?.setGravity(Gravity.BOTTOM)
        dialog?.window?.requestFeature(Window.FEATURE_NO_TITLE)
        dialog?.show()
    }

    override fun onProgressChanged(seekBar: SeekBar, progress: Int, fromUser: Boolean) {
        filterSupport.getFilter()?.setParam(progressLayout.indexOfChild(seekBar), progress)
        filterSupport.invalidate()
    }

    private fun show(count: Int) {
        oneBar.visibility = if (count > 0) View.VISIBLE else View.GONE
        twoBar.visibility = if (count > 1) View.VISIBLE else View.GONE
        thBar.visibility = if (count > 2) View.VISIBLE else View.GONE
        fBar.visibility = if (count > 3) View.VISIBLE else View.GONE
    }

    override fun onItemClick(parent: RecyclerView?, view: View?, position: Int) {
        dialog?.dismiss()
        choose(position)
    }

    private fun choose(which: Int) {
        when (which) {
            0 -> {
                filterSupport.setFilter(NormalFilter())
                show(0)
            }
            1 -> {
//                filterSupport.setFilter(BeautyV4Filter())
//                show(3)
//                oneBar.progress = 55
//                twoBar.progress = 25
//                thBar.progress = 15
                filterSupport.setFilter(HwvcFilter("$hwfDir/beauty_v4.hwf"))
                show(0)
            }
            2 -> {
                filterSupport.setFilter(HwvcFilter("$hwfDir/beach.hwf"))
                show(0)
            }
            3 -> {
                filterSupport.setFilter(HwvcFilter("$hwfDir/clean.hwf"))
                show(0)
            }
            4 -> {
                filterSupport.setFilter(HwvcFilter("$hwfDir/pink.hwf"))
                show(0)
            }
            else -> {
                filterSupport.setFilter(NormalFilter())
                show(0)
            }
        }
        filterSupport.invalidate()
    }

    override fun onStartTrackingTouch(seekBar: SeekBar?) {

    }

    override fun onStopTrackingTouch(seekBar: SeekBar?) {

    }

    private fun initResources() {
        val srcDir = "hwf"
        Log.i("HWVC", "initResources")
        val destDir = "${progressLayout.context.externalCacheDir.path}/$srcDir"
        if (!File(destDir).exists()) {
            File(destDir).mkdirs()
        }
        progressLayout.context.assets.list(srcDir).forEach {
            val destFile = File("$destDir/$it")
            if (!destFile.exists()) {
                var ins: InputStream? = null
                var fos: FileOutputStream? = null
                try {
                    ins = progressLayout.context.assets.open("$srcDir/$it")
                    fos = FileOutputStream(destFile)
                    val size = 1024 * 512
                    val buf = ByteArray(size)
                    var len: Int
                    while (true) {
                        len = ins.read(buf)
                        if (len == -1) {
                            break
                        }
                        fos.write(buf, 0, len)
                    }
                    fos.flush()
                } catch (e: Exception) {
                    e.printStackTrace()
                } finally {
                    ins?.close()
                    fos?.close()
                }
            }
        }
    }

    private class Adapter : RecyclerAdapter<FilterItem, ViewHolder>() {
        override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
            return ViewHolder(LayoutInflater.from(parent.context).inflate(R.layout.item_filter, null))
        }

        override fun onBindViewHolder(holder: ViewHolder?, item: FilterItem?, position: Int) {
            holder?.onBind(item, position)
        }

    }

    private class ViewHolder(itemView: View) : RecyclerAdapter.BaseViewHolder<FilterItem>(itemView) {
        private val nameView: TextView = itemView.findViewById(R.id.name)
        override fun onBind(item: FilterItem?, position: Int) {
            if (null == item) {
                nameView.text = "Unknown"
                return
            }
            nameView.text = item.name
        }

    }

    private data class FilterItem(var name: String,
                                  var clazz: Class<Any>?) {
        companion object {
            fun from(array: Array<String>): List<FilterItem> {
                val list = ArrayList<FilterItem>()
                array.forEach {
                    list.add(FilterItem(it, null))
                }
                return list
            }
        }
    }
}