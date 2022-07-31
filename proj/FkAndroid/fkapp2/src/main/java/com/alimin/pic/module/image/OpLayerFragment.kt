package com.alimin.pic.module.image

import android.app.Activity
import android.content.Intent
import android.view.View
import android.widget.LinearLayout
import com.alimin.pic.R
import com.alimin.pic.widget.FkContextualCommandBar
import com.alimin.pic.widget.LayerCommandItem
import com.alimin.fk.engine.FkGetLayersListener
import com.alimin.fk.engine.OnDoStatusListener
import com.alimin.fk.pb.FkImageLayerOuterClass
import com.alimin.pic.module.gallery.GalleryActivity
import com.microsoft.fluentui.contextualcommandbar.CommandItem
import com.microsoft.fluentui.contextualcommandbar.CommandItemGroup
import com.microsoft.fluentui.popupmenu.PopupMenu
import com.microsoft.fluentui.popupmenu.PopupMenuItem

class OpLayerFragment(presenter: ImageContract.Presenter) : OpFragment(presenter),
    FkContextualCommandBar.OnItemLongClickListener,
    OnLayerUpdateListener,
    OnDoStatusListener {
    override val menuResID: Int = R.menu.menu_image_op_layer
    private var pickImagePath: String? = null

    override fun initView() {
        super.initView()
        getCommandBar()?.orientation = LinearLayout.VERTICAL
        getCommandBar()?.setOnItemLongClickListener(this)
    }

    override fun onStart() {
        super.onStart()
        presenter.addLoadStatusListener(this)
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
        presenter.removeLoadStatusListener(this)
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (resultCode == Activity.RESULT_OK) {
            when (requestCode) {
                REQUEST_IMAGE -> {
                    pickImagePath = data?.getStringExtra("path")
                }
            }
        }
    }

    private fun selectLayer(layerId: Int) {
        presenter.selectLayer(layerId)
        getCommandBar()?.setItemSelected {
            return@setItemSelected if (it is LayerCommandItem && it.layerId >= 0) {
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
//                    presenter.newLayerWithColor(Size(512, 512), 255, 255, 255, 125)
                    startActivityForResult(
                        Intent(context, GalleryActivity::class.java),
                        REQUEST_IMAGE
                    )
                }
                else -> {
                    if (item.layerId >= 0) {
                        selectLayer(item.layerId)
                    }
                }
            }
        }
    }

    override fun onItemLongClick(item: CommandItem, view: View) {
        if (item is LayerCommandItem && item.layerId > 0) {
            val items = arrayListOf(
                PopupMenuItem(
                    item.layerId,
                    getString(R.string.layer_del),
                    R.drawable.ic_fluent_delete_24_regular
                ),
                PopupMenuItem(
                    item.layerId,
                    getString(R.string.layer_info),
                    R.drawable.ic_fluent_info_24_regular
                ),
            )
            val popupMenu =
                PopupMenu(requireContext(), view, items, PopupMenu.ItemCheckableBehavior.NONE)
            popupMenu.onItemClickListener = object : PopupMenuItem.OnClickListener {
                override fun onPopupMenuItemClicked(popupMenuItem: PopupMenuItem) {
                    when (popupMenuItem.iconResourceId) {
                        R.drawable.ic_fluent_delete_24_regular -> {
                            presenter.removeLayer(popupMenuItem.id)
                        }
                    }
                }
            }
            popupMenu.show()
        }
    }

    override fun onLayers(layers: List<FkImageLayerOuterClass.FkImageLayer>) {
        requireActivity().runOnUiThread(object : Runnable {
            override fun run() {
                if (layers.isEmpty()) {
                    getCommandBar()?.setMenu(menuResID)
                } else {
                    getCommandBar()?.removeItemGroup() {
                        return@removeItemGroup "layer" == it.id
                    }
                }
            }
        })
        addLayerItemView(0, getString(R.string.canvas))
        layers.forEach {
            addLayerItemView(it.id, "Layer ${it.id}: ${it.size.width}x${it.size.height}")
        }
        requireActivity().runOnUiThread(object : Runnable {
            override fun run() {
                selectLayer(-1)
            }
        })
    }

    override fun onLayer(layer: FkImageLayerOuterClass.FkImageLayer) {
    }

    private fun addLayerItemView(layerId: Int, label: String) {
        val group = CommandItemGroup().apply {
            id = "layer"
            addItem(LayerCommandItem(layerId = layerId, label = label))
        }
        requireActivity().runOnUiThread(object : Runnable {
            override fun run() {
                getCommandBar()?.addItemGroup(group)
            }
        })
    }

    companion object {
        const val REQUEST_IMAGE = 0x01
    }

    override fun onDone() {
        if (pickImagePath?.isNotEmpty() == true) {
            presenter.newLayerWithFile(pickImagePath!!)
            pickImagePath = null
        }
    }
}