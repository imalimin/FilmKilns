//package com.lmy.common.ui
//
//import android.app.Activity
//import android.content.Context
//import android.content.Intent
//import android.os.Bundle
//import android.support.v4.app.Fragment
//import android.support.v4.app.FragmentManager
//import android.support.v4.app.FragmentPagerAdapter
//import android.support.v4.view.ViewPager
//import com.lmy.common.R
//import com.lmy.common.ui.fragment.BaseLazyFragment
//import com.lmy.common.ui.fragment.GalleryImageFragment
//import java.util.*
//
///**
// * @author limingyi
// * Created on 2017/6/30 0030 14:40.
// */
//class GalleryPreviewActivity : BaseActivity(), BaseDeferFragment.OnFragmentInteractionListener {
//    companion object {
//        val KEY_PATHS = "KEY_PATHS"
//        val KEY_INDEX = "KEY_INDEX"
//
//        fun request(context: Context, paths: ArrayList<String>, index: Int) {
//            context.startActivity(Intent(context, GalleryPreviewActivity::class.java).putStringArrayListExtra(KEY_PATHS, paths)
//                    .putExtra(KEY_INDEX, index))
//        }
//    }
//
//    val mViewPager: ViewPager by bindView(R.id.pager)
//    private var mAdapter: ImagePagerAdapter? = null
//
//    override fun onCreate(savedInstanceState: Bundle?) {
//        super.onCreate(savedInstanceState)
//        initView()
//    }
//
//    override fun getLayoutView(): Int {
//        setDarkStatusBar()
//        fillStatusBar()
//        fillNavigation()
//        return R.layout.activity_gallery_preview
//    }
//
//    override fun initView() {
//        setResult(Activity.RESULT_CANCELED, Intent())
//        mAdapter = ImagePagerAdapter(intent.getStringArrayListExtra(KEY_PATHS), supportFragmentManager)
//        mViewPager.adapter = mAdapter
//        mViewPager.currentItem = intent.getIntExtra(KEY_INDEX, 0)
//        mViewPager.offscreenPageLimit = mAdapter?.count!!
//    }
//
//    override fun onFragmentInteraction(what: Int, data: Bundle?) {
//
//    }
//
//    class ImagePagerAdapter(val images: List<String>, private val fm: FragmentManager) : FragmentPagerAdapter(fm) {
//
//        override fun getItem(position: Int): Fragment {
//            val fragment = GalleryImageFragment.create(images[position])
//            return fragment
//        }
//
//        override fun getCount(): Int {
//            return images.size
//        }
//
//        override fun getPageTitle(position: Int): CharSequence {
//            return ""
//        }
//
//        fun getFragment(position: Int): Fragment {
//            return fm.findFragmentByTag("android:switcher:" + R.id.pager + ":" + position)
//        }
//
//        fun index(path: String): Int {
//            for (i in images.indices)
//                if (images[i] == path) return i
//            return 0
//        }
//    }
//}