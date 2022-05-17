package com.alimin.pic.module.main

import android.content.Intent
import android.graphics.LinearGradient
import android.graphics.Shader
import android.view.View
import androidx.recyclerview.widget.GridLayoutManager
import com.alimin.pic.R
import com.alimin.pic.model.GalleryItem
import com.alimin.pic.module.image.ImageActivity
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

        titleView.paint.shader = LinearGradient(
            0f,
            0f,
            0f,
            titleView.paint.textSize,
            resources.getColor(R.color.colorPrimaryDark),
            resources.getColor(R.color.colorAccent),
            Shader.TileMode.CLAMP
        )
        titleView.invalidate()
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
        fun newInstance() = MainFragment()
    }
}