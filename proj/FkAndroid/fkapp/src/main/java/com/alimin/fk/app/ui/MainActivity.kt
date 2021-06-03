package com.alimin.fk.app.ui

import android.content.Intent
import com.alimin.fk.app.R
import com.lmy.common.adapter.SimpleTextAdapter
import com.lmy.common.ext.setOnItemClickListener
import com.lmy.common.ui.BaseActivity
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : BaseActivity() {
    class Action(val name: String, val action: () -> Unit)

    override val layoutResID: Int = R.layout.activity_main
    private val ITEMS = arrayListOf(
        Action("Image") { startActivity(Intent(this, ImageActivity::class.java)) }
    )

    override fun initView() {
        val items = ArrayList<String>()
        ITEMS.forEach {
            items.add(it.name)
        }
        val adapter = SimpleTextAdapter()
        adapter.bindData(items)
        mRecyclerView.adapter = adapter
        mRecyclerView.setOnItemClickListener { parent, view, position ->
            ITEMS[position].action()
        }
    }
}
