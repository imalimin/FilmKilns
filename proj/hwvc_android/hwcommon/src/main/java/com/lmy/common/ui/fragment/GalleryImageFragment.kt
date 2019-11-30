//package com.lmy.common.ui.fragment
//
//import android.app.Activity
//import android.os.Bundle
//import android.view.View
//import android.widget.ProgressBar
//import com.lmy.common.R
//import com.lmy.common.widget.ExtendImageView
//import com.lmy.common.widget.subscaleview.ImageSource
//import com.lmy.common.widget.subscaleview.SubsamplingScaleImageView
//
///**
// * @author limingyi
// * Created on 2017/6/30 0030 14:27.
// */
//class GalleryImageFragment : BaseFragment<Activity>(), ExtendImageView.OnGestureListener {
//    companion object {
//        val KEY_URI = "KEY_URI"
//        fun create(path: String): GalleryImageFragment {
//            val fragment = GalleryImageFragment()
//            fragment.arguments = Bundle()
//            fragment.arguments.putString(KEY_URI, path)
//            return fragment
//        }
//    }
//
//    val mImageView: SubsamplingScaleImageView by bindView(R.id.image)
//    val mProgressBar: ProgressBar by bindView(R.id.progress)
//    override fun getLayoutView(): Int {
//        return R.layout.fragment_gallery_image
//    }
//
//    override fun initView() {
////        mImageView.setOnGestureListener(this)
//        mImageView.setImage(ImageSource.uri(arguments.getString(KEY_URI)))
//        mProgressBar.visibility = View.GONE
//    }
//
//    override fun onDestroyView() {
//        super.onDestroyView()
//        mImageView.recycle()
//    }
//
//    override fun onEvent(event: ExtendImageView.Event) {
//        when (event) {
//            ExtendImageView.Event.CLOSE -> activity.finish()
//        }
//    }
//
//}