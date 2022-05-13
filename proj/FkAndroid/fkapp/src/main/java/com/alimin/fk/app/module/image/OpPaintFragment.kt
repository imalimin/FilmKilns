package com.alimin.fk.app.module.image

import android.graphics.Color
import android.os.Bundle
import android.view.View
import com.alimin.fk.app.R
import com.microsoft.fluentui.contextualcommandbar.CommandItem
import kotlinx.android.synthetic.main.fragment_op.*

class OpPaintFragment(presenter: ImageContract.Presenter) : OpFragment(presenter) {
    override val menuResID: Int = R.menu.menu_image_op_paint
    override fun initView() {
        super.initView()
        presenter.getPaint().strokeWidth = 12
        presenter.getPaint().color = Color.YELLOW
        yesNoView.setActionListener {
            reset()
        }
    }

    override fun onItemClick(item: CommandItem, view: View) {
        when (item.getId()) {
            R.id.action_paint -> {
                getCommandBar()?.visibility = View.GONE
                yesNoView.visibility = View.VISIBLE
                onButtonPressed(R.id.action_paint, Bundle.EMPTY)
                hideBottomNav()
            }
        }
    }

    private fun reset() {
        getCommandBar()?.visibility = View.VISIBLE
        yesNoView.visibility = View.GONE
        onButtonPressed(R.id.action_paint_cancel, Bundle.EMPTY)
        showBottomNav()
    }
}