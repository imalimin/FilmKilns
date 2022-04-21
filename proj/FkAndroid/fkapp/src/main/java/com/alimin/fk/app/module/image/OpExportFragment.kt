package com.alimin.fk.app.module.image

import android.view.View
import com.alimin.fk.app.R
import com.microsoft.fluentui.contextualcommandbar.CommandItem

class OpExportFragment(presenter: ImageContract.Presenter) : OpFragment(presenter) {
    override val menuResID: Int = R.menu.menu_image_op_export
    override fun onItemClick(item: CommandItem, view: View) {
        when (item.getId()) {
            R.id.action_save_jpeg -> {
                presenter.save("/sdcard/000000.png")
            }
        }
    }
}