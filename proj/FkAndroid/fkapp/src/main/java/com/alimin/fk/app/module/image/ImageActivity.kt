package com.alimin.fk.app.module.image

import android.Manifest
import android.graphics.Point
import android.graphics.PointF
import android.os.Bundle
import android.view.*
import android.widget.FrameLayout
import androidx.core.view.forEachIndexed
import androidx.fragment.app.Fragment
import androidx.fragment.app.FragmentManager
import com.alimin.fk.app.R
import com.alimin.fk.entity.FkRational
import com.alimin.fk.widgets.FkActSurfaceView
import com.google.android.material.bottomnavigation.BottomNavigationView
import com.lmy.common.adapter.BaseViewPagerAdapter
import com.lmy.common.ext.disableStatusBarPadding
import com.lmy.common.ext.setLightMode
import com.lmy.common.ui.BaseActivity
import com.lmy.common.ui.fragment.BaseLazyFragment
import kotlinx.android.synthetic.main.activity_image.*
import pub.devrel.easypermissions.AfterPermissionGranted
import pub.devrel.easypermissions.EasyPermissions
import java.io.File

class OpPagerAdapter(private val presenter: ImageContract.Presenter, val menu:Menu, fm: FragmentManager) : BaseViewPagerAdapter(fm) {
    override fun getCount(): Int = menu.size()

    override fun getItem(position: Int): Fragment {
        return getFragment(position)
            ?: return when (menu.getItem(position).itemId) {
                R.id.action_layer -> OpLayerFragment(presenter)
                R.id.action_crop -> OpCropFragment(presenter)
                R.id.action_paint -> OpLayerFragment(presenter)
                R.id.action_filter -> OpCropFragment(presenter)
                R.id.action_save -> OpExportFragment(presenter)
                else -> OpCropFragment(presenter)
            }
    }

}

class ImageActivity : BaseActivity(),
    ImageContract.View,
    BaseLazyFragment.OnFragmentInteractionListener,
    BottomNavigationView.OnNavigationItemSelectedListener,
    SurfaceHolder.Callback,
    FkActSurfaceView.OnScrollListener,
    FkActSurfaceView.OnRotateListener,
    FkActSurfaceView.OnScaleListener,
    FkActSurfaceView.OnActionListener {
    override val layoutResID: Int = R.layout.activity_image
    override val isActive: Boolean
        get() = true
    override lateinit var presenter: ImageContract.Presenter
    private val surfaceSize = Point(0, 0)
    private var surfaceView: FkActSurfaceView? = null
    private var pickImagePath: String? = null
    private var mPagerAdapter: OpPagerAdapter? = null

    @AfterPermissionGranted(REQ_PERMISSION)
    override fun initView() {
        setLightMode(false)
        disableStatusBarPadding()
        pickImagePath = intent.getStringExtra("path")
        val workspace = File(externalCacheDir, "workspace")
        if (!workspace.exists()) {
            workspace.mkdirs()
        }
        ImagePresenter(this, workspace.absolutePath)
        val perms = arrayOf(
            Manifest.permission.WRITE_EXTERNAL_STORAGE
        )
        if (EasyPermissions.hasPermissions(this, *perms)) {
            presenter.create()
            surfaceView = FkActSurfaceView(this)
            surfaceView?.holder?.addCallback(this)
            surfaceView?.setOnScrollListener(this)
            surfaceView?.setOnScaleListener(this)
            surfaceView?.setOnRotateListener(this)
            surfaceView?.setOnActionListener(this)
            val lp = FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT)
            surfaceHolder.addView(surfaceView, lp)
        } else {
            EasyPermissions.requestPermissions(
                this, "Request write sdcard permission", REQ_PERMISSION, *perms
            )
        }
        viewPager.isEnabled = false
        navBar.setOnNavigationItemSelectedListener(this)
        mPagerAdapter = OpPagerAdapter(presenter, navBar.menu, supportFragmentManager)
        mPagerAdapter?.attach(viewPager)
    }

    fun showYesNoAction(listener: (yes: Boolean) -> Unit) {
        navBar.visibility = View.INVISIBLE
        yesNoView.visibility = View.VISIBLE
        yesNoView.setActionListener {
            listener(it)
            navBar.visibility = View.VISIBLE
            yesNoView.visibility = View.GONE
        }
    }

    override fun onStart() {
        super.onStart()
        presenter.start()
        if (pickImagePath?.isNotEmpty() == true) {
            presenter.newLayerWithFile(pickImagePath!!)
            pickImagePath = null
        }
    }

    override fun onStop() {
        super.onStop()
        presenter.stop()
    }

    override fun onDestroy() {
        super.onDestroy()
        presenter.destroy()
    }

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
        surfaceSize.x = width
        surfaceSize.y = height
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        presenter.detachFromSurface(holder.surface)
    }

    override fun surfaceCreated(holder: SurfaceHolder) {
        presenter.attachToSurface(holder.surface)
        presenter.notifyRender()
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
        presenter.postTranslate(dx.toInt(), dy.toInt())
    }

    override fun onRotate(v: SurfaceView, dr: FkRational, anchor: PointF) {
        presenter.postRotate(dr)
    }

    override fun onScale(v: SurfaceView, ds: FkRational, anchor: PointF) {
        presenter.postScale(ds)
    }

    override fun onRender(v: SurfaceView) {
        presenter.notifyRender()
    }

    override fun onNavigationItemSelected(item: MenuItem): Boolean {
        navBar.menu.forEachIndexed { index, it ->
            if (it.itemId == item.itemId) {
                viewPager.setCurrentItem(index, false)
            }
        }
        return true
    }

    override fun onFragmentInteraction(what: Int, data: Bundle) {
    }

    override fun showError(error: Int, msg: String) {
    }
}