package com.alimin.fk.app.model

data class GalleryItem(
    val isFolder: Boolean,
    var name: String,
    var path: String,
    var folderName: String
) {
    override fun equals(other: Any?): Boolean {
        if (other is GalleryItem) {
            return path == other.path
        }
        return super.equals(other)
    }
}