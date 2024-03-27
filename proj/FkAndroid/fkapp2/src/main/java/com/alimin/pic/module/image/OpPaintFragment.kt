package com.alimin.pic.module.image

import android.graphics.Color
import android.os.Bundle
import android.view.View
import com.alimin.fk.engine.OnDoStatusListener
import com.alimin.pic.R
import com.microsoft.fluentui.contextualcommandbar.CommandItem

class OpPaintFragment(presenter: ImageContract.Presenter) : OpFragment(presenter) {
    override val menuResID: Int = R.menu.menu_image_op_paint
    override fun initView() {
        super.initView()
        presenter.getPaint().strokeWidth = 12
        presenter.getPaint().color = Color.YELLOW
        mViewBinding.yesNoView.setActionListener {
            reset()
            if (!it) {
                presenter.load(object : OnDoStatusListener {
                    override fun onDone() {

                    }
                })
            }
        }
    }

    override fun onItemClick(item: CommandItem, view: View) {
        when (item.getId()) {
            R.id.action_paint -> {
                presenter.save()
                getCommandBar()?.visibility = View.GONE
                mViewBinding.yesNoView.visibility = View.VISIBLE
                onButtonPressed(R.id.action_paint, Bundle.EMPTY)
                hideBottomNav()
            }
        }
    }

    private fun reset() {
        getCommandBar()?.visibility = View.VISIBLE
        mViewBinding.yesNoView.visibility = View.GONE
        onButtonPressed(R.id.action_paint_cancel, Bundle.EMPTY)
        showBottomNav()
    }
}