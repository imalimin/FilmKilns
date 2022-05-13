package com.alimin.fk.app.module.image

import android.view.View
import com.alimin.fk.app.R
import com.microsoft.fluentui.contextualcommandbar.CommandItem
import kotlinx.android.synthetic.main.fragment_op_crop.*

class OpCropFragment(presenter: ImageContract.Presenter) : OpFragment(presenter) {
    override val menuResID: Int = R.menu.menu_image_op_crop
    override fun getLayoutView(): Int = R.layout.fragment_op_crop
    override fun onItemClick(item: CommandItem, view: View) {
        when(item.getId()) {
            R.id.action_crop -> {
                actionCrop()
            }
            R.id.action_layer_rotate -> {

            }
        }
    }

    private fun reset() {
        getCommandBar()?.visibility = View.VISIBLE
        cropView.visibility = View.GONE
        yesNoView.visibility = View.GONE
        showBottomNav()
    }

    private fun actionCrop() {
        cropView.reset()
        getCommandBar()?.visibility = View.GONE
        cropView.visibility = View.VISIBLE
        yesNoView.visibility = View.VISIBLE
        hideBottomNav()
        yesNoView.setActionListener {
            reset()
            if (it) {
                presenter.crop(cropView.getCropRect())
                presenter.notifyRender()
            }
        }
    }
}