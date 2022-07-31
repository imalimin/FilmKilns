package com.alimin.pic.module.gallery

import android.app.Activity
import android.content.Intent
import android.view.View
import androidx.core.app.ActivityCompat
import androidx.recyclerview.widget.GridLayoutManager
import com.alimin.pic.R
import com.alimin.pic.model.GalleryItem
import com.google.android.material.snackbar.Snackbar
import com.lmy.common.adapter.GalleryAdapter
import com.lmy.common.ext.setOnItemClickListener
import com.lmy.common.ui.fragment.BaseFragment
import com.lmy.mvp.util.showSnackBar
import kotlinx.android.synthetic.main.fragment_gallery.*
import java.io.File

class GalleryFragment : BaseFragment(), GalleryContract.View {
    override lateinit var presenter: GalleryContract.Presenter
    private var mAdapter: GalleryAdapter? = null

    override var isActive: Boolean = false
        get() = isAdded

    override fun getLayoutView(): Int = R.layout.fragment_gallery

    override fun initView() {
        recyclerView.layoutManager = GridLayoutManager(context, 3)
        mAdapter = GalleryAdapter()
        mAdapter?.bindData(ArrayList<File>())
        recyclerView.adapter = mAdapter
        recyclerView.setOnItemClickListener { parent, view, position ->
            with(requireActivity()) {
                setResult(Activity.RESULT_OK,
                    Intent().apply { putExtra("path", mAdapter?.getItem(position)?.absolutePath) })
                ActivityCompat.finishAfterTransition(this)
            }
        }
    }

    override fun onStart() {
        super.onStart()
        progressBar.visibility = View.VISIBLE
        presenter.queryAll(requireActivity())
    }

    override fun onStop() {
        super.onStop()
        presenter.stop()
        mAdapter?.bindData(ArrayList<File>())
    }

    override fun onShowAll(items: List<GalleryItem>) {
        progressBar.visibility = View.GONE
        mAdapter?.bindData(items.mapTo(ArrayList()) { File(it.path) })
    }

    override fun showError(error: Int, msg: String) {
        progressBar.visibility = View.GONE
        view?.showSnackBar(msg, Snackbar.LENGTH_LONG)
    }

    companion object {
        fun newInstance() = GalleryFragment()
    }
}