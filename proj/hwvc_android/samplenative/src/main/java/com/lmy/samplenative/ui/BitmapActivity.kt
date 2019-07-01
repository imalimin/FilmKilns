package com.lmy.samplenative.ui

import android.graphics.Bitmap
import com.lmy.samplenative.BaseActivity
import com.lmy.samplenative.R
import kotlinx.android.synthetic.main.activity_bitmap.*
import java.io.FileInputStream
import java.nio.ByteBuffer

class BitmapActivity : BaseActivity() {
    override fun getLayoutResource(): Int = R.layout.activity_bitmap
    override fun initView() {
        enterBtn.setOnClickListener {
            show()
        }
    }

    private fun show() {
        val width = widthEdit.text.toString().toInt()
        val height = heightEdit.text.toString().toInt()
        val path = pathEdit.text.toString()
        val buf = ByteArray(width * height * 4)
        val ios = FileInputStream(path)
        val len = ios.read(buf)
        for (i in 0 until height) {
            for (j in 0 until  width) {
                val b = buf[i * width * 4 + j * 4]
                buf[i * width * 4 + j * 4] = buf[i * width * 4 + j * 4 + 3]
                buf[i * width * 4 + j * 4 + 3] = b
            }
        }
        val bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888)
        bitmap.copyPixelsFromBuffer(ByteBuffer.wrap(buf))
        imageView.setImageBitmap(bitmap)
    }
}