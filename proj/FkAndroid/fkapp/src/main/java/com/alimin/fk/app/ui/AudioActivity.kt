package com.alimin.fk.app.ui

import android.Manifest
import android.view.View
import com.alimin.fk.app.R
import com.alimin.fk.device.FkMicrophone
import com.alimin.fk.device.FkSpeaker
import com.alimin.fk.device.FkSyncStrategy
import com.alimin.fk.entity.FkAudioSettings
import com.lmy.common.ui.BaseActivity
import kotlinx.android.synthetic.main.activity_audio.*
import pub.devrel.easypermissions.AfterPermissionGranted
import pub.devrel.easypermissions.EasyPermissions
import java.io.File

class AudioActivity : BaseActivity(), View.OnClickListener {
    companion object {
        private const val REQ_PERMISSION = 123
    }

    override val layoutResID: Int = R.layout.activity_audio
    private val mMicrophone = FkMicrophone()
    private val mSpeaker = FkSpeaker()
    private val settings = FkAudioSettings(16, 44100, 2)
    private var strategy = FkSyncStrategy()

    @AfterPermissionGranted(REQ_PERMISSION)
    override fun initView() {
        val perms = arrayOf(
            Manifest.permission.RECORD_AUDIO
        )
        if (EasyPermissions.hasPermissions(this, *perms)) {
            mRecordBtn.setOnClickListener(this)
            mMicrophone.strategy = strategy
            mSpeaker.strategy = strategy
            mMicrophone.init(settings, File(externalCacheDir, "000001.pcm").absolutePath)
            mSpeaker.init(settings, File(externalCacheDir, "test.pcm").absolutePath)
        } else {
            EasyPermissions.requestPermissions(
                this, "Request audio record permission", REQ_PERMISSION, *perms
            )
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        mMicrophone.release()
        mSpeaker.release()
    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<out String>,
        grantResults: IntArray
    ) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        EasyPermissions.onRequestPermissionsResult(requestCode, permissions, grantResults, this)
    }

    override fun onClick(v: View) {
        when (v.id) {
            R.id.mRecordBtn -> {
                mRecordBtn.isSelected = !mRecordBtn.isSelected
                if (mRecordBtn.isSelected) {
                    mMicrophone.start()
                    mSpeaker.start()
                } else {
                    mSpeaker.stop()
                    mMicrophone.stop()
                }
            }
        }
    }
}