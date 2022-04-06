package com.alimin.fk.app.module.image

import android.view.View
import com.alimin.fk.app.R
import com.alimin.fk.app.widget.setMenu
import com.lmy.common.ui.fragment.BaseFragment
import com.microsoft.fluentui.contextualcommandbar.CommandItem
import com.microsoft.fluentui.contextualcommandbar.ContextualCommandBar
import kotlinx.android.synthetic.main.fragment_op_crop.*

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

    fun getCommandBar(): ContextualCommandBar? = baseCommandBar

}