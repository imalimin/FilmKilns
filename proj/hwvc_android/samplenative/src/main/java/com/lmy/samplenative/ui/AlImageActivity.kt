package com.lmy.samplenative.ui

import android.app.Activity
import android.content.Intent
import android.util.Log
import android.view.KeyEvent
import android.view.MotionEvent
import android.view.SurfaceHolder
import android.view.View
import android.widget.SeekBar
import com.lmy.common.ui.GallerySelectActivity
import com.lmy.hwvcnative.entity.AlResult
import com.lmy.hwvcnative.processor.AlImageProcessor
import com.lmy.samplenative.BaseActivity
import com.lmy.samplenative.R
import com.microsoft.officeuifabric.bottomsheet.BottomSheetDialog
import com.microsoft.officeuifabric.bottomsheet.BottomSheetItem
import kotlinx.android.synthetic.main.activity_al_image.*

class AlImageActivity : BaseActivity(), SeekBar.OnSeekBarChangeListener,
        View.OnClickListener, View.OnTouchListener,
        BottomSheetItem.OnClickListener {
    private var bottomSheetDialog: BottomSheetDialog? = null
    private var processor: AlImageProcessor? = null
    private val mLayers = ArrayList<Int>()
    private var mCurrentLayer = -1
    private val surfaceCallback = object : SurfaceHolder.Callback {
        override fun surfaceChanged(holder: SurfaceHolder, p1: Int, p2: Int, p3: Int) {
            processor?.updateWindow(holder.surface)
        }

        override fun surfaceDestroyed(p0: SurfaceHolder?) {
            Log.i("HWVC", "surfaceDestroyed")
        }

        override fun surfaceCreated(holder: SurfaceHolder) {
            Log.i("HWVC", "surfaceCreated")
        }
    }

    override fun getLayoutResource(): Int = R.layout.activity_al_image

    override fun initView() {
        surfaceView.keepScreenOn = true
        surfaceView.holder.addCallback(surfaceCallback)
        surfaceView.setOnScrollListener { v, x, y ->
            processor?.setTranslate(mCurrentLayer,
                    x / v.width.toFloat() * 2f,
                    y / v.height.toFloat() * 2f)
        }
        GallerySelectActivity.request(this, 100, 1)
        showOpt.setOnClickListener(this)
        scaleBar.setOnSeekBarChangeListener(this)
        processor = lastCustomNonConfigurationInstance as AlImageProcessor?
        if (null == processor) {
            processor = AlImageProcessor.create()
        }
        processor?.setCanvas(1080, 1920, 0)
    }

    override fun onClick(v: View?) {
        when (v?.id) {
            R.id.showOpt -> {
                showAllOpt()
            }
        }
    }

    //Move layer
    override fun onTouch(v: View, event: MotionEvent): Boolean {
        val x = event.x / v.width.toFloat() * 2f - 1f
        val y = event.y / v.height.toFloat() * 2f - 1f
        processor?.setTranslate(mCurrentLayer, x, y)
        return true
    }

    override fun onDestroy() {
        super.onDestroy()
        surfaceView.holder.removeCallback(surfaceCallback)
    }

    //横竖屏切换
    override fun onRetainCustomNonConfigurationInstance(): Any? {
        return processor
    }

    override fun onKeyDown(keyCode: Int, event: KeyEvent?): Boolean {
        if (keyCode == KeyEvent.KEYCODE_BACK) {
            processor?.release()
        }
        return super.onKeyDown(keyCode, event)
    }

    override fun onProgressChanged(seekBar: SeekBar, progress: Int, fromUser: Boolean) {
        when (seekBar.id) {
            R.id.scaleBar ->
                processor?.setRotation(mCurrentLayer, 3.141592653f * progress / 1000)
        }
    }

    override fun onStartTrackingTouch(seekBar: SeekBar?) {
    }

    override fun onStopTrackingTouch(seekBar: SeekBar?) {
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (Activity.RESULT_OK != resultCode) {
            return
        }
        when (requestCode) {
            100 -> {
                val result = GallerySelectActivity.getResultDtata(data)
                if (null != result && result.isNotEmpty()) {
                    val layerId = processor?.addLayer(result[0])
                    if (null != layerId && layerId >= 0) {
                        mLayers.add(layerId)
                        mCurrentLayer = layerId
                        Log.i("HWVC", "addLayer $layerId")
                    }
                }
            }
        }
    }

    private val OPTS = arrayListOf<BottomSheetItem>(
            BottomSheetItem(0, R.mipmap.ic_launcher, "Add Layer"),
            BottomSheetItem(1, R.mipmap.ic_launcher, "Show All Layer"))

    override fun onBottomSheetItemClick(item: BottomSheetItem) {
        when (item.id) {
            0 -> {
                GallerySelectActivity.request(this, 100, 1)
            }
            1 -> {
                showAllLayers()
            }
        }
    }

    private fun showAllOpt() {
        bottomSheetDialog = BottomSheetDialog(this, OPTS)
        bottomSheetDialog?.onItemClickListener = this
        bottomSheetDialog?.show()
    }

    private fun showAllLayers() {
        val list = mLayers.mapTo(ArrayList()) { BottomSheetItem(it, R.mipmap.ic_launcher, "Layer $it") }
        val dialog = BottomSheetDialog(this, list)
        dialog.onItemClickListener = object : BottomSheetItem.OnClickListener {
            override fun onBottomSheetItemClick(item: BottomSheetItem) {
                if (AlResult.SUCCESS == processor?.removeLayer(item.id)) {
                    for (i in 0 until mLayers.size) {
                        if (mLayers[i] == item.id) {
                            mLayers.removeAt(i)
                        }

                    }
                }
            }
        }
        dialog.show()
    }
}