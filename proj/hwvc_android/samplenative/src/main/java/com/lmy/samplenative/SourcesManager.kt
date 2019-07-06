package com.lmy.samplenative

import android.annotation.SuppressLint
import android.content.Context
import android.os.AsyncTask
import android.support.v7.app.AlertDialog
import android.util.Log
import java.io.File
import java.io.FileOutputStream
import java.io.InputStream
import java.lang.Exception

class SourcesManager(private val context: Context) {
    private var initDialog: AlertDialog? = null
    private var task: AsyncTask<Void, Void, Void?>? = null

    fun initialize() {
        initDialog = AlertDialog.Builder(context)
                .setMessage("Initializing...")
                .setCancelable(false)
                .show()
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

    private fun initResources() {
        Log.i("HWVC", "initResources")
        val srcHwfDir = "hwf"
        val destHwfDir = "${context.externalCacheDir.path}/$srcHwfDir"
        copyAssets(srcHwfDir, destHwfDir)
        val srcImageDir = "image"
        val destImageDir = "${context.externalCacheDir.path}/$srcImageDir"
        copyAssets(srcImageDir, destImageDir)
        val srcVideoDir = "video"
        val destVideoDir = "${context.externalCacheDir.path}/$srcVideoDir"
        copyAssets(srcVideoDir, destVideoDir)
    }

    private fun copyAssets(srcAsset: String, destCacheDir: String) {
        if (!File(destCacheDir).exists()) {
            File(destCacheDir).mkdirs()
        }
        context.assets.list(srcAsset).forEach {
            val destFile = File("$destCacheDir/$it")
            if (!destFile.exists()) {
                var ins: InputStream? = null
                var fos: FileOutputStream? = null
                try {
                    ins = context.assets.open("$srcAsset/$it")
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