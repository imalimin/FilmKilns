package com.alimin.pic.module.image

import android.os.Bundle
import android.view.View
import androidx.viewbinding.ViewBinding
import com.alimin.pic.R
import com.alimin.pic.databinding.FragmentOpBinding
import com.alimin.pic.widget.FkContextualCommandBar
import com.lmy.common.ui.fragment.BaseFragment
import com.microsoft.fluentui.contextualcommandbar.CommandItem

abstract class OpFragment(val presenter: ImageContract.Presenter) : BaseFragment(), CommandItem.OnItemClickListener {
    protected abstract val menuResID: Int
    override fun getLayoutView(): Int = R.layout.fragment_op
    protected lateinit var mViewBinding: FragmentOpBinding
    override fun getViewBinding(): ViewBinding = FragmentOpBinding.inflate(layoutInflater).apply {
        mViewBinding = this
    }

    override fun initView() {
        with(mViewBinding.baseCommandBar) {
            setMenu(menuResID)
            setItemOnClickListener(object : CommandItem.OnItemClickListener {
                override fun onItemClick(item: CommandItem, view: View) {
                    this@OpFragment.onItemClick(item, view)
                }
            })
        }
    }

    fun getCommandBar(): FkContextualCommandBar? = mViewBinding.baseCommandBar

    fun hideBottomNav() {
        onButtonPressed(R.id.action_hide_bottom_nav, Bundle.EMPTY)
    }

    fun showBottomNav() {
        onButtonPressed(R.id.action_show_bottom_nav, Bundle.EMPTY)
    }

}