package com.alimin.fk.app.module.image

import android.view.View
import com.alimin.fk.app.R
import com.alimin.fk.engine.FkImage
import com.microsoft.fluentui.contextualcommandbar.CommandItem

class OpCropFragment(engine: FkImage) : OpFragment(engine) {
    override val menuResID: Int = R.menu.menu_image_op_crop
    override fun onItemClick(item: CommandItem, view: View) {
    }
}