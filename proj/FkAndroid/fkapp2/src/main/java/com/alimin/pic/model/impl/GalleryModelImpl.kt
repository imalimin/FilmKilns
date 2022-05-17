package com.alimin.pic.model.impl

import android.database.Cursor
import android.os.Bundle
import android.provider.MediaStore
import androidx.fragment.app.FragmentActivity
import androidx.loader.app.LoaderManager
import androidx.loader.content.CursorLoader
import androidx.loader.content.Loader
import com.alimin.pic.model.GalleryItem
import com.alimin.pic.model.GalleryModel
import com.alimin.pic.model.entity.Error
import java.io.File
import java.util.concurrent.locks.ReentrantLock
import kotlin.collections.LinkedHashSet

class GalleryModelImpl : GalleryModel {
    private var loader: LoaderManager? = null
    private val galleryItems = LinkedHashSet<GalleryItem>()
    private val queryLock = ReentrantLock()
    override fun <T> queryAll(
        owner: T,
        result: (data: List<GalleryItem>, error: Error) -> Unit
    ) where T : FragmentActivity {
        if (queryLock.tryLock()) {
            if (galleryItems.isEmpty()) {
                queryLock.unlock()
                queryAllFromDatabase(owner, result)
            } else {
                val list = galleryItems.toList()
                queryLock.unlock()
                result(list, Error.success())
            }
        } else {
            queryAllFromDatabase(owner, result)
        }
    }

    private fun <T> queryAllFromDatabase(
        owner: T,
        result: (data: List<GalleryItem>, error: Error) -> Unit
    ) where T : FragmentActivity {
        if (loader == null) {
            loader = LoaderManager.getInstance(owner)
            loader?.initLoader(LOADER_ALL, Bundle.EMPTY, newCallback(owner, result))
        } else {
            loader?.restartLoader<Cursor>(
                LOADER_ALL,
                Bundle.EMPTY,
                newCallback(owner, result)
            )
        }
    }

    private fun <T> newCallback(
        owner: T,
        result: (data: List<GalleryItem>, error: Error) -> Unit
    ): LoaderManager.LoaderCallbacks<Cursor> where T : FragmentActivity {
        return object : LoaderManager.LoaderCallbacks<Cursor> {
            override fun onCreateLoader(id: Int, args: Bundle?): Loader<Cursor> {
                return newLoaderCursor(owner, id, args)
            }

            override fun onLoadFinished(loader: Loader<Cursor>, data: Cursor?) {
                if (data != null && data.count > 0) {
                    data.moveToFirst()
                    queryLock.lock()
                    galleryItems.clear()
                    do {
                        val path = data.getString(data.getColumnIndexOrThrow(IMAGE_PROJECTION[0]))
                        val image = File(path)
                        val name = data.getString(data.getColumnIndexOrThrow(IMAGE_PROJECTION[1]))
//                        val dateTime = data.getLong(data.getColumnIndexOrThrow(IMAGE_PROJECTION[2]))
                        if (image.exists()) {
                            galleryItems.add(GalleryItem(false, name, path, image.parentFile.name))
                        }
                    } while (data.moveToNext())
                    queryLock.unlock()
                    result(galleryItems.toList(), Error.success())
                } else {
                    result(ArrayList<GalleryItem>(), Error(-1, "Gallery is empty"))
                }
            }

            override fun onLoaderReset(loader: Loader<Cursor>) {
            }
        }
    }

    private fun <T> newLoaderCursor(
        owner: T,
        id: Int,
        args: Bundle?
    ): Loader<Cursor> where T : FragmentActivity {
        return when (id) {
            LOADER_ALL -> newAllLoader(owner, args)
            LOADER_CATEGORY -> newCategoryLoader(owner, args)
            else -> CursorLoader(owner)
        }
    }

    private fun <T> newAllLoader(
        owner: T,
        args: Bundle?
    ): Loader<Cursor> where T : FragmentActivity = CursorLoader(
        owner,
        MediaStore.Images.Media.EXTERNAL_CONTENT_URI, IMAGE_PROJECTION,
        "${IMAGE_PROJECTION[4]}>0 AND ${IMAGE_PROJECTION[3]}=? OR ${IMAGE_PROJECTION[3]}=? ",
        arrayOf("image/jpeg", "image/png"), "${IMAGE_PROJECTION[2]} DESC"
    )

    private fun <T> newCategoryLoader(
        owner: T,
        args: Bundle?
    ): Loader<Cursor> where T : FragmentActivity = CursorLoader(
        owner,
        MediaStore.Images.Media.EXTERNAL_CONTENT_URI, IMAGE_PROJECTION,
        IMAGE_PROJECTION[4] + ">0 AND " + IMAGE_PROJECTION[0] + " like '%" + args?.getString(
            "path"
        ) + "%'", emptyArray(),
        IMAGE_PROJECTION[2] + " DESC"
    )

    companion object {
        private val LOADER_ALL = 0
        private val LOADER_CATEGORY = 1
        private val IMAGE_PROJECTION = arrayOf(
            MediaStore.Images.Media.DATA,
            MediaStore.Images.Media.DISPLAY_NAME,
            MediaStore.Images.Media.DATE_ADDED,
            MediaStore.Images.Media.MIME_TYPE,
            MediaStore.Images.Media.SIZE,
            MediaStore.Images.Media.RELATIVE_PATH,
            MediaStore.Images.Media._ID
        )
    }
}