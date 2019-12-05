package com.lmy.samplenative.ui

import android.app.Activity
import android.content.Intent
import android.media.MediaScannerConnection
import android.net.Uri
import android.os.Environment
import android.util.Log
import android.view.KeyEvent
import android.view.SurfaceHolder
import android.view.View
import android.widget.SeekBar
import android.widget.Toast
import com.lmy.common.ui.GallerySelectActivity
import com.lmy.hwvcnative.entity.AlRational
import com.lmy.hwvcnative.processor.AlImageProcessor
import com.lmy.samplenative.BaseActivity
import com.lmy.samplenative.R
import com.microsoft.officeuifabric.bottomsheet.BottomSheetDialog
import com.microsoft.officeuifabric.bottomsheet.BottomSheetItem
import kotlinx.android.synthetic.main.activity_al_image.*
import java.io.File

class AlImageActivity : BaseActivity(), SeekBar.OnSeekBarChangeListener,
        View.OnClickListener, BottomSheetItem.OnClickListener,
        AlImageProcessor.OnSaveListener {

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
        surfaceView?.setOnClickListener { v, x, y ->
            if (null != processor) {
                setCurLayer(processor!!.getLayer(x, y))
            }
        }
        surfaceView.setOnScrollListener { v, x, y, dx, dy ->
            processor?.postTranslate(mCurrentLayer, dx, dy)
        }
        surfaceView?.setOnScaleListener { v, ds ->
            processor?.postScale(mCurrentLayer, ds)
        }
        surfaceView?.setOnRotateListener { v, dr ->
            processor?.postRotation(mCurrentLayer, dr)
        }
        showOpt.setOnClickListener(this)
        processor = lastCustomNonConfigurationInstance as AlImageProcessor?
        if (null == processor) {
            processor = AlImageProcessor.create()
        }
        processor?.setCanvas(1080, 1920)
        processor?.setOnSaveListener(this)
        pickImage()
    }

    override fun onClick(v: View?) {
        when (v?.id) {
            R.id.showOpt -> {
                showAllOpt()
            }
        }
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
            REQUEST_IMAGE -> {
                val result = GallerySelectActivity.getResultDtata(data)
                if (null != result && result.isNotEmpty()) {
                    val layerId = processor?.addLayer(result[0])
                    if (null != layerId && layerId >= 0) {
                        mLayers.add(layerId)
                        setCurLayer(layerId)
                        Log.i("HWVC", "addLayer $layerId")
                    }
                }
            }
        }
    }

    private fun setCurLayer(layer: Int) {
        mCurrentLayer = layer
        layerView.text = mCurrentLayer.toString()
    }

    private fun pickImage() {
        GallerySelectActivity.request(this, REQUEST_IMAGE, 1)
    }

    override fun onSave(code: Int, msg: String?, path: String?) {
        Toast.makeText(this@AlImageActivity,
                "Save finish: $code",
                Toast.LENGTH_LONG).show()
        if (null != path) {
            MediaScannerConnection.scanFile(this, Array<String>(1) { path }, null) { _: String, uri: Uri ->
                val mediaScanIntent = Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE)
                mediaScanIntent.data = uri
                sendBroadcast(mediaScanIntent)
            }
        }
    }

    private val OPTS = arrayListOf<BottomSheetItem>(
            BottomSheetItem(0, R.mipmap.ic_launcher, "Add Layer"),
            BottomSheetItem(10, R.mipmap.ic_launcher, "Rest Layer"),
            BottomSheetItem(100, R.mipmap.ic_launcher, "Delete Layer"),
            BottomSheetItem(200, R.mipmap.ic_launcher, "Save"))

    override fun onBottomSheetItemClick(item: BottomSheetItem) {
        when (item.id) {
            0 -> {
                pickImage()
            }
            10 -> {
                processor?.setTranslate(mCurrentLayer, 0f, 0f)
                processor?.setRotation(mCurrentLayer, AlRational.zero())
                processor?.setScale(mCurrentLayer, AlRational(1, 1))
            }
            100 -> {
                processor?.removeLayer(mCurrentLayer)
            }
            200 -> {
                processor?.save("${File(Environment.getExternalStorageDirectory(),
                        "alimage.jpg").absoluteFile}")
            }
        }
    }

    private fun showAllOpt() {
        bottomSheetDialog = BottomSheetDialog(this, OPTS)
        bottomSheetDialog?.onItemClickListener = this
        bottomSheetDialog?.show()
    }

    companion object {
        const val REQUEST_IMAGE = 100
    }
}