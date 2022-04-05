package com.alimin.fk.app.model.impl

import android.database.Cursor
import android.os.Bundle
import android.provider.MediaStore
import androidx.fragment.app.FragmentActivity
import androidx.loader.app.LoaderManager
import androidx.loader.content.CursorLoader
import androidx.loader.content.Loader
import com.alimin.fk.app.model.GalleryItem
import com.alimin.fk.app.model.GalleryModel
import com.alimin.fk.app.model.entity.Error
import java.io.File

class GalleryModelImpl : GalleryModel {
    private var loader: LoaderManager? = null
    private var folders = HashMap<String, File>()
    override fun <T> queryAll(
        owner: T,
        result: (data: List<GalleryItem>, error: Error) -> Unit
    ) where T : FragmentActivity {
        if (loader == null) {
            loader = LoaderManager.getInstance(owner)
            loader?.initLoader(LOADER_ALL, Bundle.EMPTY, newCallback(owner, result))
        } else {
            loader?.restartLoader<Cursor>(LOADER_ALL, Bundle.EMPTY, newCallback(owner, result))
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
                    val items = ArrayList<GalleryItem>()
                    do {
                        val path = data.getString(data.getColumnIndexOrThrow(IMAGE_PROJECTION[0]))
                        val image = File(path)
                        val name = data.getString(data.getColumnIndexOrThrow(IMAGE_PROJECTION[1]))
//                        val dateTime = data.getLong(data.getColumnIndexOrThrow(IMAGE_PROJECTION[2]))
                        if (image.exists()) {
                            items.add(GalleryItem(false, name, path))
                        }
                        if (folders.isEmpty()) {
                            // 获取文件夹名称
                            val folderFile = File(path).parentFile
                            if (folderFile != null && folderFile.exists()) {
                                val fp = folderFile.absolutePath
                                folders[fp] = folderFile
                            }
                        }
                    } while (data.moveToNext())
                    result(items, Error.success())
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
        arrayOf("image/jpeg", "image/png"),  "${IMAGE_PROJECTION[2]} DESC"
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