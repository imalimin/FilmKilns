package com.alimin.fk.app.module.image

import android.util.Size
import android.view.View
import android.widget.LinearLayout
import com.alimin.fk.app.R
import com.alimin.fk.app.widget.FkContextualCommandBar
import com.alimin.fk.app.widget.LayerCommandItem
import com.alimin.fk.engine.FkGetLayersListener
import com.alimin.fk.pb.FkImageLayerOuterClass
import com.microsoft.fluentui.contextualcommandbar.CommandItem
import com.microsoft.fluentui.contextualcommandbar.CommandItemGroup
import kotlinx.android.synthetic.main.fragment_op.*

class OpLayerFragment(presenter: ImageContract.Presenter) : OpFragment(presenter),
    FkContextualCommandBar.OnItemLongClickListener,
    OnLayerUpdateListener {
    override val menuResID: Int = R.menu.menu_image_op_layer

    override fun initView() {
        super.initView()
        getCommandBar()?.orientation = LinearLayout.VERTICAL
        getCommandBar()?.setOnItemLongClickListener(this)
    }

    override fun onStart() {
        super.onStart()
        presenter.addLayerUpdateListener(this)
        presenter.getLayers(object : FkGetLayersListener {
            override fun onGetLayers(layers: List<FkImageLayerOuterClass.FkImageLayer>) {
                onLayers(layers)
            }
        })
    }

    override fun onStop() {
        super.onStop()
        presenter.removeLayerUpdateListener(this)
    }

    private fun selectLayer(layerId: Int) {
        presenter.selectLayer(layerId)
        getCommandBar()?.setItemSelected {
            return@setItemSelected if (it is LayerCommandItem) {
                presenter.isSelectedLayer(it.layerId)
            } else {
                false
            }
        }
    }

    override fun onItemClick(item: CommandItem, view: View) {
        if (item is LayerCommandItem) {
            when (item.getId()) {
                R.id.action_layer_add -> {
                    presenter.newLayerWithColor(Size(512, 512), 255, 255, 255, 125)
                }
                else -> {
                    if (item.layerId > 0) {
                        selectLayer(item.layerId)
                    }
                }
            }
        }
    }

    override fun onItemLongClick(item: CommandItem, view: View) {
        if (item is LayerCommandItem && item.layerId > 0) {
            presenter.removeLayer(item.layerId)
        }
    }

    override fun onLayers(layers: List<FkImageLayerOuterClass.FkImageLayer>) {
        requireActivity().runOnUiThread(object : Runnable {
            override fun run() {
                getCommandBar()?.removeItemGroup() {
                    return@removeItemGroup "layer" == it.id
                }
            }
        })
        layers.forEach {
            val label = "Layer ${it.id}: ${it.size.width}x${it.size.height}"
            val group = CommandItemGroup().apply {
                id = "layer"
                addItem(LayerCommandItem(layerId = it.id, label = label))
            }
            requireActivity().runOnUiThread(object : Runnable {
                override fun run() {
                    getCommandBar()?.addItemGroup(group)
                }
            })
        }
        requireActivity().runOnUiThread(object : Runnable {
            override fun run() {
                selectLayer(-1)
            }
        })
    }

    override fun onLayer(layer: FkImageLayerOuterClass.FkImageLayer) {
    }
}