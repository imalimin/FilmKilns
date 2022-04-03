package com.alimin.fk.app.module.main

import com.alimin.fk.app.R
import com.lmy.common.ui.fragment.BaseFragment

class MainFragment : BaseFragment(), MainContract.View {
    override lateinit var presenter: MainContract.Presenter
    override var isActive: Boolean = false
        get() = isAdded

    override fun getLayoutView(): Int = R.layout.fragment_main

    override fun initView() {
//        presenter.queryHotVideoList(0)
//        recyclerView.layoutManager = LinearLayoutManager(context)
//        recyclerView.adapter = adapter
//        recyclerView.setOnItemClickListener { _, _, position ->
//            VideoActivity.intent(context!!, adapter.items[position].id)
//        }
//        signButton.setOnClickListener {
//            SignActivity.intent(activity!!, 0x1)
//        }
    }

    override fun onResume() {
        super.onResume()
        presenter.start()
    }

    override fun showError(error: Int, msg: String) {
    }

    companion object {
        fun newInstance() = MainFragment()
    }
}