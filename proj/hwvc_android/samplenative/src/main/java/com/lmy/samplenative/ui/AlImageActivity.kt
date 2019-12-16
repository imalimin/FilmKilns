package com.lmy.samplenative.ui

import android.app.Activity
import android.content.Context
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
        View.OnClickListener, AlImageProcessor.OnSaveListener {

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
        showOptLayer(false)
        surfaceView.keepScreenOn = true
        surfaceView.holder.addCallback(surfaceCallback)
        surfaceView?.setOnClickListener { v, x, y ->
            if (null != processor) {
                setCurLayer(processor!!.getLayer(x, y))
            }
        }
        surfaceView.setOnScrollListener { v, x, y, dx, dy ->
            //            processor?.postTranslate(mCurrentLayer, dx, dy)
        }
        surfaceView?.setOnScaleListener { v, ds ->
            //            processor?.postScale(mCurrentLayer, ds)
        }
        surfaceView?.setOnRotateListener { v, dr ->
            processor?.postRotation(mCurrentLayer, dr)
        }
        optBtn.setOnClickListener(this)
        fileBtn.setOnClickListener(this)
        canvasBtn.setOnClickListener(this)
        layerBtn.setOnClickListener(this)
        processor = lastCustomNonConfigurationInstance as AlImageProcessor?
        if (null == processor) {
            processor = AlImageProcessor.create()
        }
//        processor?.setCanvas(1080, 1920)
        processor?.setOnSaveListener(this)
        pickImage()
    }

    override fun onClick(v: View?) {
        when (v?.id) {
            R.id.fileBtn -> {
                bottomSheetDialog = FileOptDialog(this, processor).show()
            }
            R.id.canvasBtn -> {
                bottomSheetDialog = CanvasOptDialog(this, processor).show()
            }
            R.id.layerBtn -> {
                bottomSheetDialog = LayerOptDialog(this, processor).show()
            }
            R.id.optBtn -> {
                optLayout.visibility = if (View.VISIBLE == optLayout.visibility) {
                    View.GONE
                } else {
                    View.VISIBLE
                }
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
//                    processor?.setRotation(mCurrentLayer, AlRational(1,4))
                }
            }
        }
    }

    private fun setCurLayer(layer: Int) {
        mCurrentLayer = layer
        layerView.text = mCurrentLayer.toString()
    }

    fun pickImage() {
        GallerySelectActivity.request(this, REQUEST_IMAGE, 1)
    }

    fun getCurrentLayer(): Int = mCurrentLayer

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

    fun showOptLayer(show: Boolean) {
        optLayout.visibility = if (show) View.VISIBLE else View.GONE
    }

    companion object {
        const val REQUEST_IMAGE = 100
    }
}

interface IOptDialog {
    fun show(): BottomSheetDialog
}

class FileOptDialog(private var context: Context, private var processor: AlImageProcessor?)
    : IOptDialog, BottomSheetItem.OnClickListener {
    private val OPTS = arrayListOf<BottomSheetItem>(
            BottomSheetItem(0, R.mipmap.ic_launcher, "Save"),
            BottomSheetItem(1, R.mipmap.ic_launcher, "None"),
            BottomSheetItem(2, R.mipmap.ic_launcher, "None")
    )

    override fun show(): BottomSheetDialog {
        val dialog = BottomSheetDialog(context, OPTS)
        dialog.onItemClickListener = this
        dialog.show()
        return dialog
    }

    override fun onBottomSheetItemClick(item: BottomSheetItem) {
        when (item.id) {
            0 -> {
                processor?.save("${File(Environment.getExternalStorageDirectory(),
                        "alimage.jpg").absoluteFile}")
            }
        }
    }

}

class CanvasOptDialog(private var context: Context, private var processor: AlImageProcessor?)
    : IOptDialog, BottomSheetItem.OnClickListener {
    private val OPTS = arrayListOf<BottomSheetItem>(
            BottomSheetItem(0, R.mipmap.ic_launcher, "Change Canvas"),
            BottomSheetItem(1, R.mipmap.ic_launcher, "None"),
            BottomSheetItem(2, R.mipmap.ic_launcher, "None")
    )

    override fun show(): BottomSheetDialog {
        val dialog = BottomSheetDialog(context, OPTS)
        dialog.onItemClickListener = this
        dialog.show()
        return dialog
    }

    override fun onBottomSheetItemClick(item: BottomSheetItem) {
        when (item.id) {
            0 -> {
                processor?.setCanvas(1080, 1080)
            }
        }
    }
}

class LayerOptDialog(private var context: AlImageActivity, private var processor: AlImageProcessor?)
    : IOptDialog, BottomSheetItem.OnClickListener {
    private val OPTS = arrayListOf<BottomSheetItem>(
            BottomSheetItem(0, R.mipmap.ic_launcher, "Add Layer"),
            BottomSheetItem(1, R.mipmap.ic_launcher, "Rest Layer"),
            BottomSheetItem(2, R.mipmap.ic_launcher, "Delete Layer"),
            BottomSheetItem(3, R.mipmap.ic_launcher, "Crop Layer")
    )

    override fun show(): BottomSheetDialog {
        val dialog = BottomSheetDialog(context, OPTS)
        dialog.onItemClickListener = this
        dialog.show()
        return dialog
    }

    override fun onBottomSheetItemClick(item: BottomSheetItem) {
        when (item.id) {
            0 -> {
                context.pickImage()
            }
            1 -> {
                processor?.setTranslate(context.getCurrentLayer(), 0f, 0f)
                processor?.setRotation(context.getCurrentLayer(), AlRational.zero())
                processor?.setScale(context.getCurrentLayer(), AlRational(1, 1))
            }
            2 -> {
                processor?.removeLayer(context.getCurrentLayer())
            }
            3 -> {
                if (View.VISIBLE != context.cropView.visibility) {
                    context.cropView.visibility = View.VISIBLE
                } else {
                    context.showOptLayer(false)
                    context.cropView.visibility = View.GONE
                    val rectF = context.cropView.getCropRectF()
                    processor?.cropLayer(context.getCurrentLayer(),
                            rectF.left, rectF.top,
                            rectF.right, rectF.bottom)
                }
            }
        }
    }
}