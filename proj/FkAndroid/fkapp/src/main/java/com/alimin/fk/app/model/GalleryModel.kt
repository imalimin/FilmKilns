package com.alimin.fk.app.model

import androidx.fragment.app.FragmentActivity
import com.alimin.fk.app.model.entity.Error

interface GalleryModel {
    fun <T> queryAll(
        owner: T,
        result: (data: List<GalleryItem>, error: Error) -> Unit
    ) where T : FragmentActivity
}