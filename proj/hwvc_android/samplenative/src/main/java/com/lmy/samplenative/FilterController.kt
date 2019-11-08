package com.lmy.samplenative

import android.annotation.SuppressLint
import android.content.Context
import android.os.AsyncTask
import android.support.v7.app.AlertDialog
import android.util.Log
import android.view.View
import android.view.ViewGroup
import android.widget.SeekBar
import com.lmy.hwvcnative.FilterSupport
import com.lmy.hwvcnative.filter.HwNormalFilter
import com.lmy.hwvcnative.filter.HwvcFilter
import com.microsoft.officeuifabric.bottomsheet.BottomSheetDialog
import com.microsoft.officeuifabric.bottomsheet.BottomSheetItem
import java.io.File
import java.io.FileOutputStream
import java.io.InputStream
import java.util.*

/**
 * Created by aliminabc@gmail.com on 2018/7/24.
 */
class FilterController(private val filterSupport: FilterSupport,
                       private val progressLayout: ViewGroup)
    : SeekBar.OnSeekBarChangeListener, BottomSheetItem.OnClickListener {

    companion object {
        private val FILTERS = arrayOf(
                "Normal", "Beauty V4", "Beach", "Clean", "Pink", "Abaose", "Rise")
    }

    private var oneBar: SeekBar = progressLayout.getChildAt(0) as SeekBar
    private var twoBar: SeekBar = progressLayout.getChildAt(1) as SeekBar
    private var thBar: SeekBar = progressLayout.getChildAt(2) as SeekBar
    private var fBar: SeekBar = progressLayout.getChildAt(3) as SeekBar
    private var bottomSheetDialog: BottomSheetDialog? = null
    private var initDialog: AlertDialog? = null
    private var task: AsyncTask<Void, Void, Void?>? = null
    private var hwfDir: String = "${progressLayout.context.externalCacheDir!!.path}/hwf"

    init {
        oneBar.setOnSeekBarChangeListener(this)
        twoBar.setOnSeekBarChangeListener(this)
        thBar.setOnSeekBarChangeListener(this)
        fBar.setOnSeekBarChangeListener(this)
//        initDialog = AlertDialog.Builder(progressLayout.context)
//                .setMessage("Initializing...")
//                .setCancelable(false)
//                .show()
        @SuppressLint("CI_StaticFieldLeak")
        task = object : AsyncTask<Void, Void, Void?>() {
            override fun doInBackground(vararg params: Void?): Void? {
                initResources()
                return null
            }

            override fun onPostExecute(result: Void?) {
                super.onPostExecute(result)
                initDialog?.dismiss()
            }
        }
        task?.execute()
    }

    fun chooseFilter(context: Context) {
        val list = ArrayList<BottomSheetItem>()
        FILTERS.forEachIndexed { index, it ->
            list.add(BottomSheetItem(index, R.mipmap.ic_launcher, it))
        }
        bottomSheetDialog = BottomSheetDialog(context, list)
        bottomSheetDialog?.onItemClickListener = this
        bottomSheetDialog?.show()
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

    override fun onBottomSheetItemClick(item: BottomSheetItem) {
        choose(item.id)
    }

    private fun choose(which: Int) {
        when (which) {
            0 -> {
                filterSupport.setFilter(HwNormalFilter())
                show(0)
            }
            1 -> {
//                filterSupport.setFilter(BeautyV4Filter())
//                show(3)
//                oneBar.progress = 55
//                twoBar.progress = 25
//                thBar.progress = 15
                filterSupport.setFilter(HwvcFilter("$hwfDir/beauty_v4.hwf", true))
                show(0)
            }
            2 -> {
                filterSupport.setFilter(HwvcFilter("$hwfDir/beach.hwf", true))
                show(0)
            }
            3 -> {
                filterSupport.setFilter(HwvcFilter("$hwfDir/clean.hwf", true))
                show(0)
            }
            4 -> {
                filterSupport.setFilter(HwvcFilter("$hwfDir/pink.hwf", true))
                show(0)
            }
            5 -> {
                filterSupport.setFilter(HwvcFilter("$hwfDir/abaose.hwf", true))
                show(0)
            }
            6 -> {
                filterSupport.setFilter(HwvcFilter("$hwfDir/rise.hwf", true))
                show(0)
            }
            else -> {
                filterSupport.setFilter(HwNormalFilter())
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
        Log.i("HWVC", "initResources")
        val srcHwfDir = "hwf"
        val destHwfDir = "${progressLayout.context.externalCacheDir!!.path}/$srcHwfDir"
        copyAssets(srcHwfDir, destHwfDir)
        val srcImageDir = "image"
        val destImageDir = "${progressLayout.context.externalCacheDir!!.path}/$srcImageDir"
        copyAssets(srcImageDir, destImageDir)
    }

    private fun copyAssets(srcAsset: String, destCacheDir: String) {
        if (!File(destCacheDir).exists()) {
            File(destCacheDir).mkdirs()
        }
        progressLayout.context.assets.list(srcAsset)?.forEach {
            val destFile = File("$destCacheDir/$it")
            if (!destFile.exists()) {
                var ins: InputStream? = null
                var fos: FileOutputStream? = null
                try {
                    ins = progressLayout.context.assets.open("$srcAsset/$it")
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
}