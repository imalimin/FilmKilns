package com.alimin.fk.app.model.impl

import com.alimin.fk.app.model.GalleryItem
import com.alimin.fk.app.model.GalleryModel

class GalleryModelImpl : GalleryModel {
    override fun queryAll(vid: Long, result: (data: List<GalleryItem>, error: Error?) -> Unit) {
        TODO("Not yet implemented")
    }
}