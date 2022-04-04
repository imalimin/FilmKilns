package com.alimin.fk.app.module.main

import android.Manifest
import android.os.Bundle
import com.alimin.fk.app.R
import com.lmy.common.ext.disableStatusBarPadding
import com.lmy.common.ext.setLightMode
import com.lmy.common.ui.BaseActivity
import com.lmy.common.ui.fragment.BaseLazyFragment
import com.lmy.mvp.util.replaceFragmentInActivity
import pub.devrel.easypermissions.AfterPermissionGranted
import pub.devrel.easypermissions.EasyPermissions

class MainActivity : BaseActivity(), BaseLazyFragment.OnFragmentInteractionListener {
    override val layoutResID: Int = R.layout.activity_fragment_container

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setLightMode(false)
        disableStatusBarPadding()
    }

    @AfterPermissionGranted(REQ_PERMISSION)
    override fun initView() {
        val perms = arrayOf(
            Manifest.permission.WRITE_EXTERNAL_STORAGE
        )
        if (EasyPermissions.hasPermissions(this, *perms)) {
            val fragment = supportFragmentManager.findFragmentById(R.id.fragment_container) as MainFragment?
                ?: MainFragment.newInstance().also {
                    replaceFragmentInActivity(it, R.id.fragment_container)
                }
            MainPresenter(fragment)
        } else {
            EasyPermissions.requestPermissions(
                this, "Request write sdcard permission", REQ_PERMISSION, *perms
            )
        }
    }

    override fun onFragmentInteraction(what: Int, data: Bundle) {

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
    }
}