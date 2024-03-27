package com.alimin.pic.ui

import android.content.Intent
import androidx.viewbinding.ViewBinding
import com.alimin.pic.R
import com.alimin.pic.databinding.ActivityMainBinding
import com.alimin.pic.module.image.ImageActivity
import com.lmy.common.adapter.SimpleTextAdapter
import com.lmy.common.databinding.ActivityGallerySelectBinding
import com.lmy.common.ext.setOnItemClickListener
import com.lmy.common.ui.BaseActivity

class MainActivity : BaseActivity() {
    class Action(val name: String, val action: () -> Unit)

    override val layoutResID: Int = R.layout.activity_main
    private val ITEMS = arrayListOf(
        Action("Image") { startActivity(Intent(this, ImageActivity::class.java)) },
        Action("Audio") { startActivity(Intent(this, AudioActivity::class.java)) }
    )
    private lateinit var mViewBinding: ActivityMainBinding
    override fun getViewBinding(): ViewBinding = ActivityMainBinding.inflate(layoutInflater).apply {
        mViewBinding = this
    }

    override fun initView() {
        val items = ArrayList<String>()
        ITEMS.forEach {
            items.add(it.name)
        }
        val adapter = SimpleTextAdapter()
        adapter.bindData(items)
        mViewBinding.mRecyclerView.adapter = adapter
        mViewBinding.mRecyclerView.setOnItemClickListener { parent, view, position ->
            ITEMS[position].action()
        }
    }
}
