package com.lmy.samplenative.ui

import android.view.TextureView
import android.widget.FrameLayout
import com.lmy.hwvcnative.processor.HwCameraRecorder
import com.lmy.samplenative.BaseActivity
import com.lmy.samplenative.R
import kotlinx.android.synthetic.main.activity_camera.*

class CameraActivity : BaseActivity() {
    private var recorder: HwCameraRecorder? = HwCameraRecorder()
    override fun getLayoutResource(): Int = R.layout.activity_camera
    override fun initView() {
        //Init TextureView
        val mTextureView = TextureView(this).apply {
            fitsSystemWindows = true
            keepScreenOn = true
//            setOnTouchListener(this@CameraActivity)
        }
        textureContainer.addView(mTextureView, FrameLayout.LayoutParams(FrameLayout.LayoutParams.MATCH_PARENT,
                FrameLayout.LayoutParams.MATCH_PARENT))
        recorder?.startPreview(mTextureView)
    }
}