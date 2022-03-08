package com.alimin.fk.app.ui

import android.Manifest
import android.content.ContentUris
import android.content.Intent
import android.database.Cursor
import android.graphics.PointF
import android.net.Uri
import android.provider.DocumentsContract
import android.provider.MediaStore
import android.view.SurfaceHolder
import android.view.SurfaceView
import android.view.ViewGroup
import android.widget.FrameLayout
import android.widget.Toast
import com.alimin.fk.app.R
import com.alimin.fk.engine.FkImage
import com.alimin.fk.entity.FkRational
import com.alimin.fk.widgets.FkActSurfaceView
import com.lmy.common.ui.BaseActivity
import kotlinx.android.synthetic.main.activity_image.*
import pub.devrel.easypermissions.AfterPermissionGranted
import pub.devrel.easypermissions.EasyPermissions


class ImageActivity : BaseActivity(),
    SurfaceHolder.Callback,
    FkActSurfaceView.OnScrollListener,
    FkActSurfaceView.OnRotateListener,
    FkActSurfaceView.OnScaleListener,
    FkActSurfaceView.OnActionListener {
    override val layoutResID: Int = R.layout.activity_image
    private var surfaceView: FkActSurfaceView? = null
    private var engine: FkImage = FkImage()
    private var layer = -1

    @AfterPermissionGranted(REQ_PERMISSION)
    override fun initView() {
        val perms = arrayOf(
            Manifest.permission.WRITE_EXTERNAL_STORAGE
        )
        if (EasyPermissions.hasPermissions(this, *perms)) {
            engine.create()
            surfaceView = FkActSurfaceView(this)
            surfaceView?.holder?.addCallback(this)
            surfaceView?.setOnScrollListener(this)
            surfaceView?.setOnScaleListener(this)
            surfaceView?.setOnRotateListener(this)
            surfaceView?.setOnActionListener(this)
            val lp = FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT)
            surfaceHolder.addView(surfaceView, lp)
            pickPicture()
        } else {
            EasyPermissions.requestPermissions(
                this, "Request write sdcard permission", REQ_PERMISSION, *perms
            )
        }
    }

    private fun pickPicture() {
        val intent = Intent(Intent.ACTION_PICK, MediaStore.Images.Media.EXTERNAL_CONTENT_URI)
        startActivityForResult(intent, REQ_PICK_PICTURE)
    }

    private fun pickPictureAction(uri: Uri) {
        var imagePath: String? = null
        if (DocumentsContract.isDocumentUri(this, uri)) {
            val docId = DocumentsContract.getDocumentId(uri)
            if ("com.android.providers.media.documents" == uri.authority) {
                val id = docId.split(":").toTypedArray()[1]
                val selection = MediaStore.Images.Media._ID + "=" + id
                imagePath = getImagePath(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, selection)
            } else if ("com.android.providers.downloads.documents" == uri.getAuthority()) {
                val contentUri: Uri = ContentUris.withAppendedId(
                    Uri.parse("content://downloads/public_downloads"),
                    java.lang.Long.valueOf(docId)
                )
                imagePath = getImagePath(contentUri, null)
            }
        } else if ("content".equals(uri.scheme, ignoreCase = true)) {
            imagePath = getImagePath(uri, null)
        } else if ("file".equals(uri.scheme, ignoreCase = true)) {
            imagePath = uri.path
        }

        if (imagePath?.isNotEmpty() == true) {
            engine.start()
            layer = engine.newLayerWithFile(imagePath)
            engine.notifyRender()
        }else {
            Toast.makeText(this, "File Not Found!", Toast.LENGTH_LONG).show()
        }
    }

    private fun getImagePath(uri: Uri, selection: String?): String? {
        var path: String? = null
        val cursor: Cursor? = contentResolver.query(uri, null, selection, null, null)
        if (cursor != null) {
            if (cursor.moveToFirst()) {
                path = cursor.getString(cursor.getColumnIndex(MediaStore.Images.Media.DATA))
            }
            cursor.close()
        }
        return path
    }

    override fun onDestroy() {
        super.onDestroy()
        engine.destroy()
    }

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        engine.stop()
        engine.detachFromSurface(holder.surface)
    }

    override fun surfaceCreated(holder: SurfaceHolder) {
        engine.start()
        engine.attachToSurface(holder.surface)
        engine.notifyRender()
//        engine.setCanvasSize(512, 512)
//        layer = engine.newLayerWithColor(512,512, 0,255,255, 255)
//        Log.i("FilmKilns", "newLayer: $layer")
//        engine.drawPoint(0, 0xff0000, 300, 300)
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        when (requestCode) {
            REQ_PICK_PICTURE -> if (data != null) pickPictureAction(data.data!!)
        }
    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<out String>,
        grantResults: IntArray
    ) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        EasyPermissions.onRequestPermissionsResult(requestCode, permissions, grantResults, this)
    }

    companion object {
        private const val REQ_PERMISSION = 123
        private const val REQ_PICK_PICTURE = 124
    }

    override fun onScroll(v: SurfaceView, x: Float, y: Float, dx: Float, dy: Float, status: Int) {
        engine.postTranslate(layer, (dx * v.measuredWidth / 2).toInt(), (-dy * v.measuredHeight / 2).toInt())
    }

    override fun onRotate(v: SurfaceView, dr: FkRational, anchor: PointF) {
        engine.postRotation(layer, dr.num, dr.den)
    }

    override fun onScale(v: SurfaceView, ds: FkRational, anchor: PointF) {
        engine.postScale(layer, ds.num.toFloat() / ds.den, ds.num.toFloat() / ds.den)
    }

    override fun onRender(v: SurfaceView) {
        engine.notifyRender()
    }
}