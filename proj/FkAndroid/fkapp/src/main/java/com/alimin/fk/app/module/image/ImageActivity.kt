package com.alimin.fk.app.module.image

import android.Manifest
import android.graphics.*
import android.os.Bundle
import android.view.*
import android.widget.FrameLayout
import androidx.core.view.forEachIndexed
import androidx.fragment.app.Fragment
import androidx.fragment.app.FragmentManager
import com.alimin.fk.app.R
import com.alimin.fk.engine.FkImage
import com.alimin.fk.engine.FkImageFile
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

class OpPagerAdapter(engine: FkImage, val menu:Menu, fm: FragmentManager) : BaseViewPagerAdapter(fm) {
    override fun getCount(): Int = menu.size()

    override fun getItem(position: Int): Fragment {
        return getFragment(position)
            ?: return when (menu.getItem(position).itemId) {
                R.id.action_layer -> OpLayerFragment()
                R.id.action_crop -> OpCropFragment()
                R.id.action_paint -> OpLayerFragment()
                R.id.action_filter -> OpCropFragment()
                R.id.action_save -> OpLayerFragment()
                else -> OpCropFragment()
            }
    }

}

class ImageActivity : BaseActivity(),
    BaseLazyFragment.OnFragmentInteractionListener,
    BottomNavigationView.OnNavigationItemSelectedListener,
    SurfaceHolder.Callback,
    FkActSurfaceView.OnScrollListener,
    FkActSurfaceView.OnRotateListener,
    FkActSurfaceView.OnScaleListener,
    FkActSurfaceView.OnActionListener {
    override val layoutResID: Int = R.layout.activity_image
    private val surfaceSize = Point(0, 0)
    private var surfaceView: FkActSurfaceView? = null
    private lateinit var cacheFile: File
    private lateinit var engine: FkImage
    private lateinit var fileEngine: FkImageFile
    private var layer = -1
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
        cacheFile = File(externalCacheDir, "/${System.currentTimeMillis()}.fkp")
        engine = FkImage(workspace.absolutePath)
        fileEngine = FkImageFile(engine)
        val perms = arrayOf(
            Manifest.permission.WRITE_EXTERNAL_STORAGE
        )
        if (EasyPermissions.hasPermissions(this, *perms)) {
            engine.create()
            fileEngine.create()
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
        navBar.setOnNavigationItemSelectedListener(this)
        mPagerAdapter = OpPagerAdapter(engine, navBar.menu, supportFragmentManager)
        mPagerAdapter?.attach(viewPager)
//        testBtn.setOnClickListener {
////            val point = Point(800, 1500)
////            engine.drawPoint(layer, 0xFFFF0000, 30, point)
////            engine.drawPoint(layer, 0xFFFF0000, 30, Point(0, 0))
////            coverView.showPoint(point)
//            val lt = Point(surfaceSize.x / 2 - 100, surfaceSize.y / 2 - 150)
//            val rb = Point(surfaceSize.x / 2 + 150, surfaceSize.y / 2 + 200)
//            engine.drawPoint(layer, 0xFFFF0000, 30, lt)
//            engine.drawPoint(layer, 0xFFFF0000, 30, rb)
//            engine.crop(layer, lt, rb)
//            engine.notifyRender()
//        }
    }

    override fun onStart() {
        super.onStart()
        engine.start()
        fileEngine.start()
        fileEngine.load(cacheFile.absolutePath)
        if (pickImagePath?.isNotEmpty() == true) {
            layer = engine.newLayerWithFile(pickImagePath!!)
            engine.notifyRender()
            pickImagePath = null
        }
    }

    override fun onStop() {
        super.onStop()
        fileEngine.save(cacheFile.absolutePath)
        fileEngine.stop()
        engine.stop()
    }

    override fun onDestroy() {
        super.onDestroy()
        fileEngine.destroy()
        engine.destroy()
    }

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
        surfaceSize.x = width
        surfaceSize.y = height
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        engine.detachFromSurface(holder.surface)
    }

    override fun surfaceCreated(holder: SurfaceHolder) {
        engine.attachToSurface(holder.surface)
        engine.notifyRender()
//        engine.setCanvasSize(512, 512)
//        layer = engine.newLayerWithColor(512,512, 0,255,255, 255)
//        Log.i("FilmKilns", "newLayer: $layer")
//        engine.drawPoint(0, 0xff0000, 300, 300)
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
        engine.postTranslate(layer, dx.toInt(), dy.toInt())
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
}