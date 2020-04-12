package com.lmy.common.model.entity

import android.text.TextUtils

import java.io.File

/**
 * @author limingyi
 * Created on 2017/5/19 0019 11:19.
 */

class Folder {
    var name: String? = null
    var path: String? = null
    var cover: File? = null
    var images: ArrayList<File>? = null

    override fun equals(o: Any?): Boolean {
        try {
            val other = o as Folder?
            return TextUtils.equals(other!!.path, path)
        } catch (e: ClassCastException) {
            e.printStackTrace()
        }

        return super.equals(o)
    }

    companion object {

        fun create(name: String, path: String, cover: File): Folder {
            val folder = Folder()
            folder.name = name
            folder.path = path
            folder.cover = cover
            return folder
        }
    }
}
