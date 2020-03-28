package com.lmy.samplenative.ui

import android.app.Activity
import android.content.Intent
import android.graphics.Point
import android.graphics.PointF
import android.graphics.RectF
import android.media.MediaScannerConnection
import android.net.Uri
import android.os.Bundle
import android.os.Environment
import android.text.TextUtils
import android.util.Log
import android.view.KeyEvent
import android.view.SurfaceHolder
import android.view.View
import android.widget.CompoundButton
import android.widget.SeekBar
import android.widget.Toast
import com.lmy.common.ui.GallerySelectActivity
import com.lmy.common.ui.fragment.BaseLazyFragment
import com.lmy.file.ui.dialog.FileDialog
import com.lmy.hwvcnative.entity.AlRational
import com.lmy.hwvcnative.entity.AlResult
import com.lmy.hwvcnative.processor.AlImageProcessor
import com.lmy.samplenative.BaseActivity
import com.lmy.samplenative.R
import com.microsoft.officeuifabric.bottomsheet.BottomSheetDialog
import com.microsoft.officeuifabric.bottomsheet.BottomSheetItem
import kotlinx.android.synthetic.main.activity_al_image.*
import java.io.File

class AlImageActivity : BaseActivity(), BaseLazyFragment.OnFragmentInteractionListener,
        SeekBar.OnSeekBarChangeListener, CompoundButton.OnCheckedChangeListener,
        View.OnClickListener, AlImageProcessor.OnSaveListener {
    private var bottomSheetDialog: BottomSheetDialog? = null
    private var processor: AlImageProcessor? = null
    private val mLayers = ArrayList<Int>()
    private var mCurrentLayer = -1
    private var alpha: Double = 0.0
    private var outputName: String? = null
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
        surfaceView.setOnScrollListener { v, x, y, dx, dy, s ->
            if (!alignCropBox.isChecked && !paintBox.isChecked) {
                processor?.postTranslate(getCurrentLayer(), dx, dy)
            }
            if (paintBox.isChecked) {
                processor?.paint(getCurrentLayer(), PointF(x, y), 1 == s)
            }
            //For crop debug
//            ensureCropLayer()
        }
        surfaceView?.setOnScaleListener { v, ds, anchor ->
            if (!alignCropBox.isChecked && !paintBox.isChecked) {
                processor?.postScale(getCurrentLayer(), ds, anchor)
            }
            //For crop debug
//            ensureCropLayer()
        }
        surfaceView?.setOnRotateListener { v, dr, anchor ->
            if (!alignCropBox.isChecked && !paintBox.isChecked) {
                processor?.postRotation(getCurrentLayer(), dr, anchor)
            } else {
                alpha += (dr.num / dr.den.toDouble())
                processor?.ensureAlignCrop(getCurrentLayer(),
                        AlRational((alpha * 100000).toInt(), 100000))
            }
        }
        optBtn.setOnClickListener(this)
        fileBtn.setOnClickListener(this)
        editBtn.setOnClickListener(this)
        canvasBtn.setOnClickListener(this)
        layerBtn.setOnClickListener(this)
        alignCropBox.setOnCheckedChangeListener(this)
        selectBox.setOnCheckedChangeListener(this)
        processor = lastCustomNonConfigurationInstance as AlImageProcessor?
        if (null == processor) {
            processor = AlImageProcessor.create()
        }
//        processor?.setCanvas(1920, 1080)
        processor?.setOnSaveListener(this)
        //For crop debug
//        cropView.setOnChangeListener {
//            processor?.cancelCropLayer(getCurrentLayer())
//            val rectF = getSelectRect()
//            if (null != rectF) {
//                processor?.ensureCropLayer(getCurrentLayer(),
//                        rectF.left, rectF.top,
//                        rectF.right, rectF.bottom)
//            }
//        }
        openDefaultImage()
    }

    private fun openDefaultImage() {
        var uri = intent.data
        if (uri == null) {
            uri = intent.getParcelableExtra(Intent.EXTRA_STREAM)
        }
        if (null == uri) {
            pickImage()
            return
        }
        val path = getRealFilePath(uri)
        if (TextUtils.isEmpty(path)) {
            Toast.makeText(this, "没有找到该文件", Toast.LENGTH_SHORT).show()
            return
        }
        addLayer(path!!)
    }

    override fun onClick(v: View?) {
        when (v?.id) {
            R.id.fileBtn -> {
                bottomSheetDialog = FileOptDialog(this, processor).show()
            }
            R.id.canvasBtn -> {
                bottomSheetDialog = CanvasOptDialog(this, processor).show()
            }
            R.id.editBtn -> {
                bottomSheetDialog = EditOptDialog(this, processor).show()
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

    override fun onCheckedChanged(v: CompoundButton, isChecked: Boolean) {
        when (v.id) {
            R.id.alignCropBox -> {
                if (isChecked) {
                    alpha = 0.0
                    processor?.ensureAlignCrop(getCurrentLayer(), AlRational(0, 100000))
                } else {
                    processor?.cancelAlignCrop(getCurrentLayer())
                }
            }
            R.id.selectBox -> {
                showSelector(v.isChecked)
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
                    addLayer(result[0])
                }
            }
        }
    }

    override fun onFragmentInteraction(what: Int, data: Bundle) {
        when (what) {
            REQUEST_FILE -> {
                Toast.makeText(this, data.getString("path"), Toast.LENGTH_LONG).show()
                val path = data.getString("path")
                if (!TextUtils.isEmpty(path)) {
                    outputName = path!!
                    processor?.import(path!!)
                }
            }
        }
    }

    private fun addLayer(file: String) {
        val layerId = processor?.addLayer(file)
        if (null == outputName) {
            outputName = File(file).name
        }
        if (null != layerId && layerId >= 0) {
            mLayers.add(layerId)
            setCurLayer(layerId)
            Log.i("HWVC", "addLayer $layerId")
        }
    }

    private fun setCurLayer(layer: Int) {
        mCurrentLayer = layer
        layerView.text = layer.toString()
    }

    fun pickImage() {
        GallerySelectActivity.request(this, REQUEST_IMAGE, 1)
    }

    fun pickFile() {
        val selector = FileDialog.newInstance(REQUEST_FILE)
        selector.show(supportFragmentManager, "SELECTOR")
    }

    fun getCurrentLayer(): Int = mCurrentLayer

    fun getOutputName(): String? = outputName

    override fun onSave(code: Int, msg: String?, path: String?) {
        Toast.makeText(this@AlImageActivity,
                "Save finish: $code",
                Toast.LENGTH_LONG).show()
        if (null != path || code != AlResult.SUCCESS) {
            MediaScannerConnection.scanFile(this, Array<String>(1) { path!! }, null) { _: String, uri: Uri ->
                val mediaScanIntent = Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE)
                mediaScanIntent.data = uri
                sendBroadcast(mediaScanIntent)
            }
        }
    }

    fun showOptLayer(show: Boolean) {
//        optLayout.visibility = if (show) View.VISIBLE else View.GONE
        optLayout.visibility = View.VISIBLE
    }

    fun showSelector(show: Boolean) {
        selectBox.isChecked = show
        cropView.visibility = if (show) View.VISIBLE else View.GONE
        cropView.reset()
    }

    fun getSelectRect(): RectF? {
        if (View.VISIBLE != cropView.visibility) return null
        return cropView.getCropRectF()
    }

    companion object {
        const val REQUEST_IMAGE = 100
        const val REQUEST_FILE = 200
    }
}

interface IOptDialog {
    fun show(): BottomSheetDialog
}

class FileOptDialog(private var context: AlImageActivity, private var processor: AlImageProcessor?)
    : IOptDialog, BottomSheetItem.OnClickListener {
    private val OPTS = arrayListOf<BottomSheetItem>(
            BottomSheetItem(0, R.mipmap.ic_launcher, "Save"),
            BottomSheetItem(1, R.mipmap.ic_launcher, "Export"),
            BottomSheetItem(2, R.mipmap.ic_launcher, "Import")
    )

    override fun show(): BottomSheetDialog {
        val dialog = BottomSheetDialog(context, OPTS)
        dialog.onItemClickListener = this
        dialog.show()
        return dialog
    }

    override fun onBottomSheetItemClick(item: BottomSheetItem) {
        val parent = File(Environment.getExternalStorageDirectory(), "Pictures")
        if (!parent.exists()) {
            parent.mkdirs()
        }
        when (item.id) {
            0 -> {
                var outputName = context.getOutputName()
                if (null != outputName) {
                    var count = 0
                    while (true) {
                        if (File(parent, "$outputName").exists()) {
                            ++count
                            val suffix = outputName!!.substring(outputName.lastIndexOf("."), outputName.length)
                            outputName = "${outputName.substring(0, outputName.lastIndexOf("."))}-${count}$suffix"
                            continue
                        }
                        break
                    }
                    processor?.save(File(parent, "$outputName").absolutePath)
                } else {
                    Toast.makeText(context,
                            "Save finish failed. Add a layer first. Pls",
                            Toast.LENGTH_LONG).show()
                }
            }
            1 -> {
                val outputName = context.getOutputName()
                if (null != outputName) {
                    val name = outputName.substring(0, outputName.lastIndexOf('.'))
                    processor?.export("${File(parent, "${name}.alx").absoluteFile}")
                    Toast.makeText(context, "Export finish.", Toast.LENGTH_LONG).show()
                } else {
                    Toast.makeText(context,
                            "Save finish failed. Add a layer first. Pls",
                            Toast.LENGTH_LONG).show()
                }
            }
            2 -> {
                context.pickFile()
            }
        }
    }

}

class EditOptDialog(private var context: AlImageActivity, private var processor: AlImageProcessor?)
    : IOptDialog, BottomSheetItem.OnClickListener {
    private val OPTS = arrayListOf<BottomSheetItem>(
            BottomSheetItem(0, R.mipmap.ic_launcher, "Select"),
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
                context.showSelector(context.cropView.visibility != View.VISIBLE)
            }
        }
    }

}

class CanvasOptDialog(private var context: AlImageActivity, private var processor: AlImageProcessor?)
    : IOptDialog, BottomSheetItem.OnClickListener {
    private val OPTS = arrayListOf<BottomSheetItem>(
            BottomSheetItem(0, R.mipmap.ic_launcher, "Crop Canvas"),
            BottomSheetItem(1, R.mipmap.ic_launcher, "Resize to 720p"),
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
                val rectF = context.getSelectRect()
                if (null != rectF) {
                    processor?.cropCanvas(rectF.left, rectF.top,
                            rectF.right, rectF.bottom)
                }
                context.showSelector(false)
            }
            1 -> {
                processor?.setCanvas(1280, 720)
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
            BottomSheetItem(3, R.mipmap.ic_launcher, "Crop Layer"),
            BottomSheetItem(4, R.mipmap.ic_launcher, "Clear Crop Layer")
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
                processor?.cancelCropLayer(context.getCurrentLayer())
                val rectF = context.getSelectRect()
                if (null != rectF) {
                    processor?.ensureCropLayer(context.getCurrentLayer(),
                            rectF.left, rectF.top,
                            rectF.right, rectF.bottom)
                }
                context.showSelector(false)
            }
            4 -> {
                processor?.cancelCropLayer(context.getCurrentLayer())
            }
        }
    }
}