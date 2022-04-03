package com.alimin.fk.app.module.main

import android.content.Intent
import androidx.recyclerview.widget.GridLayoutManager
import com.alimin.fk.app.R
import com.alimin.fk.app.model.GalleryItem
import com.alimin.fk.app.ui.ImageActivity
import com.google.android.material.snackbar.Snackbar
import com.lmy.common.adapter.GalleryAdapter
import com.lmy.common.ext.setOnItemClickListener
import com.lmy.common.ui.fragment.BaseFragment
import com.lmy.mvp.util.showSnackBar
import kotlinx.android.synthetic.main.fragment_main.*
import java.io.File

class MainFragment : BaseFragment(), MainContract.View {
    override lateinit var presenter: MainContract.Presenter
    private var mAdapter: GalleryAdapter? = null

    override var isActive: Boolean = false
        get() = isAdded

    override fun getLayoutView(): Int = R.layout.fragment_main

    override fun initView() {
        recyclerView.layoutManager = GridLayoutManager(context, 3)
        mAdapter = GalleryAdapter()
        mAdapter?.bindData(ArrayList<File>())
        recyclerView.adapter = mAdapter
        recyclerView.setOnItemClickListener { parent, view, position ->
            startActivity(Intent(requireActivity(), ImageActivity::class.java).apply {
                putExtra("path", mAdapter?.getItem(position)?.absolutePath)
            })
        }
    }

    override fun onStart() {
        super.onStart()
        presenter.start()
        presenter.queryAll(requireActivity())
    }

    override fun onStop() {
        super.onStop()
        presenter.stop()
    }

    override fun onShowAll(items: List<GalleryItem>) {
        mAdapter?.bindData(items.mapTo(ArrayList()) { File(it.path) })
    }

    override fun showError(error: Int, msg: String) {
        view?.showSnackBar(msg, Snackbar.LENGTH_LONG)
    }

    companion object {
        fun newInstance() = MainFragment()
    }
}