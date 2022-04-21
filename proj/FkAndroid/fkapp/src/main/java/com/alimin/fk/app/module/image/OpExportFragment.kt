package com.alimin.fk.app.module.image

import android.view.View
import com.alimin.fk.app.R
import com.microsoft.fluentui.contextualcommandbar.CommandItem
import java.io.File

class OpExportFragment(presenter: ImageContract.Presenter) : OpFragment(presenter) {
    override val menuResID: Int = R.menu.menu_image_op_export
    override fun onItemClick(item: CommandItem, view: View) {
        val name = "/sdcard/DCIM/Camera/${System.currentTimeMillis()}"
        when (item.getId()) {
            R.id.action_save_jpeg -> {
                presenter.save("${name}.jpg")
            }
            R.id.action_save_png -> {
                presenter.save("${name}.png")
            }
            R.id.action_save_webp -> {
                presenter.save("${name}.webp")
            }
        }
    }
}