package com.lmy.samplenative

import android.content.ContentResolver
import android.net.Uri
import android.os.Build
import android.os.Bundle
import android.os.Environment
import android.provider.MediaStore
import android.text.TextUtils
import android.util.Log
import android.view.View
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import com.lmy.samplenative.helper.PermissionHelper
import java.util.*

/**
 * Created by aliminabc@gmail.com on 2018/9/21.
 */
open abstract class BaseActivity : AppCompatActivity() {
    abstract fun initView()
    abstract fun getLayoutResource(): Int
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(getLayoutResource())
        fillStatusBar()
        if (!PermissionHelper.requestPermissions(this, PermissionHelper.PERMISSIONS_BASE))
            return
        initView()
    }

    private fun showPermissionsDialog() {
        AlertDialog.Builder(this)
                .setMessage("Please grant permission in the permission settings")
                .setNegativeButton("cancel") { dialog, which -> finish() }
                .setPositiveButton("enter") { dialog, which ->
                    PermissionHelper.gotoPermissionManager(this@BaseActivity)
                    finish()
                }
                .show()
    }

    override fun onRequestPermissionsResult(requestCode: Int, permissions: Array<String>, grantResults: IntArray) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        if (null == grantResults || grantResults.isEmpty()) return
        when (requestCode) {
            PermissionHelper.REQUEST_MY -> {
                if (PermissionHelper.checkGrantResults(grantResults)) {
                    initView()
                } else {
                    showPermissionsDialog()
                }
            }
        }
    }

    fun fillStatusBar() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            window.decorView.systemUiVisibility = (window.decorView.systemUiVisibility
                    or View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                    or View.SYSTEM_UI_FLAG_LAYOUT_STABLE)
        }
    }

    fun getRealFilePath(uri: Uri?): String? {
        if (null == uri) return null
        Log.i("BaseActivity", uri.toString())
        Log.i("BaseActivity", uri.path)
        val scheme = uri.scheme
        var data: String? = null
        if (scheme == null)
            data = uri.path
        else if (ContentResolver.SCHEME_FILE == scheme) {
            data = uri.path
        } else if (ContentResolver.SCHEME_CONTENT == scheme) {
            val cursor = contentResolver.query(uri, null, null, null, null)
            if (null != cursor) {
                if (cursor.moveToFirst()) {
                    val columns = cursor.columnNames
                    Log.i("BaseActivity", Arrays.toString(columns))
                    val index = cursor.getColumnIndex(MediaStore.MediaColumns.DATA)
                    val name = cursor.getString(cursor.getColumnIndex(MediaStore.MediaColumns.DISPLAY_NAME))
                    val size = cursor.getString(cursor.getColumnIndex(MediaStore.MediaColumns.SIZE))
                    if (index > -1) {
                        data = cursor.getString(index)
                    } else if (!TextUtils.isEmpty(uri.path) && uri.path!!.contains(name)) {
                        //投机处理
                        val path = uri.path!!.substring(1)
                        data = "${Environment.getExternalStorageDirectory().path}${path.substring(path.indexOf("/"), path.length)}"
                    }
                }
                cursor.close()
            }
        }
        return data
    }
}