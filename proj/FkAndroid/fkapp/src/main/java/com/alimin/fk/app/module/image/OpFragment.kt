package com.alimin.fk.app.module.image

import android.os.Bundle
import android.view.View
import com.alimin.fk.app.R
import com.alimin.fk.app.widget.FkContextualCommandBar
import com.lmy.common.ui.fragment.BaseFragment
import com.microsoft.fluentui.contextualcommandbar.CommandItem
import kotlinx.android.synthetic.main.fragment_op.*

abstract class OpFragment(val presenter: ImageContract.Presenter) : BaseFragment(), CommandItem.OnItemClickListener {
    protected abstract val menuResID: Int
    override fun getLayoutView(): Int = R.layout.fragment_op

    override fun initView() {
        with(baseCommandBar) {
            setMenu(menuResID)
            setItemOnClickListener(object : CommandItem.OnItemClickListener {
                override fun onItemClick(item: CommandItem, view: View) {
                    this@OpFragment.onItemClick(item, view)
                }
            })
        }
    }

    fun getCommandBar(): FkContextualCommandBar? = baseCommandBar

    fun hideBottomNav() {
        onButtonPressed(R.id.action_hide_bottom_nav, Bundle.EMPTY)
    }

    fun showBottomNav() {
        onButtonPressed(R.id.action_show_bottom_nav, Bundle.EMPTY)
    }

}