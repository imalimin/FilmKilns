package com.alimin.fk.app.module.image

import android.view.View
import com.alimin.fk.app.R
import com.alimin.fk.engine.FkGetLayersListener
import com.alimin.fk.pb.FkImageLayerOuterClass
import com.microsoft.fluentui.contextualcommandbar.CommandItem
import com.microsoft.fluentui.contextualcommandbar.CommandItemGroup
import com.microsoft.fluentui.contextualcommandbar.DefaultCommandItem

class OpLayerFragment(presenter: ImageContract.Presenter) : OpFragment(presenter),
    OnLayerUpdateListener {
    override val menuResID: Int = R.menu.menu_image_op_layer

    override fun initView() {
        super.initView()
        presenter
    }

    override fun onStart() {
        super.onStart()
        presenter.getLayers(object : FkGetLayersListener {
            override fun onGetLayers(layers: List<FkImageLayerOuterClass.FkImageLayer>) {
                onLayers(layers)
            }
        })
    }

    override fun onItemClick(item: CommandItem, view: View) {
    }

    override fun onLayers(layers: List<FkImageLayerOuterClass.FkImageLayer>) {
        layers.forEach {
            val label = "Layer: ${it}"
            getCommandBar()?.addItemGroup(CommandItemGroup().addItem(DefaultCommandItem(label = label)))
        }
    }

    override fun onLayer(layer: FkImageLayerOuterClass.FkImageLayer) {
    }

}