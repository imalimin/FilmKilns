package com.alimin.pic.module.image

import android.view.View
import androidx.viewbinding.ViewBinding
import com.alimin.pic.R
import com.alimin.pic.databinding.FragmentOpCropBinding
import com.microsoft.fluentui.contextualcommandbar.CommandItem

class OpCropFragment(presenter: ImageContract.Presenter) : OpFragment(presenter) {
    override val menuResID: Int = R.menu.menu_image_op_crop
    override fun getLayoutView(): Int = R.layout.fragment_op_crop
    protected lateinit var mViewBinding2: FragmentOpCropBinding
    override fun getViewBinding(): ViewBinding = FragmentOpCropBinding.inflate(layoutInflater).apply {
        mViewBinding2 = this
    }
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
        mViewBinding2.cropView.visibility = View.GONE
        mViewBinding2.yesNoView.visibility = View.GONE
        showBottomNav()
    }

    private fun actionCrop() {
        mViewBinding2.cropView.reset()
        getCommandBar()?.visibility = View.GONE
        mViewBinding2.cropView.visibility = View.VISIBLE
        mViewBinding2.yesNoView.visibility = View.VISIBLE
        hideBottomNav()
        mViewBinding2.yesNoView.setActionListener {
            reset()
            if (it) {
                presenter.crop(mViewBinding2.cropView.getCropRect())
                presenter.notifyRender()
            }
        }
    }
}