package com.alimin.fk.app.model

interface GalleryModel {
    fun queryAll(vid: Long, result: (data: List<GalleryItem>, error: Error?) -> Unit)
}