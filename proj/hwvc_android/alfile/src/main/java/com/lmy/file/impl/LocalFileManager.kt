package com.lmy.file.impl

import android.os.Environment
import android.text.TextUtils
import com.lmy.common.ext.copy
import com.lmy.common.ext.delete
import com.lmy.common.ext.fileName
import com.lmy.common.ext.move
import com.lmy.file.FileManager
import com.lmy.file.entity.Setting
import java.io.File

class LocalFileManager internal constructor(private var isChildManager: Boolean = false)
    : FileManager {
    private val storages = ArrayList<String>()
    private var path: String = "/"
    private var setting = Setting()

    init {
        storages.add(Environment.getExternalStorageDirectory().path)
    }

    override fun setSetting(setting: Setting) {
        setting.copyTo(this.setting)
    }

    override fun getSetting(): Setting = setting

    override fun of(path: String): FileManager {
        return LocalFileManager(true).apply { open(path) }
    }

    private fun sort(list: Array<String>): Array<String> {
        if (list.isEmpty()) {
            return list
        }
        list.sortWith(Comparator { o1, o2 ->
            val f1 = File(this.path, o1)
            val f2 = File(this.path, o2)
            val sort = if (Setting.Sort.ASC == setting.sort) {
                o1.toLowerCase().compareTo(o2.toLowerCase())
            } else {
                o2.toLowerCase().compareTo(o1.toLowerCase())
            }
            return@Comparator if (setting.group) {
                if (f1.isFile && f2.isDirectory) {
                    1
                } else if (f1.isDirectory && f2.isFile) {
                    -1
                } else {
                    sort
                }
            } else {
                sort
            }
        })
        return list
    }

    private fun getPath(name: String): String {
        return "$this.path/$name"
    }

    private fun filter(list: Array<String>): Array<String> {
        return filter(list, setting.showHide)
    }

    @Synchronized
    override fun open(name: String): Array<String>? {
        val file = File(this.path, name)
        if (!file.exists() || !file.isDirectory || !file.canRead()) {
            return null
        }
        this.path = file.path
        return sort(filter((file.list())))
    }

    @Throws
    @Synchronized
    override fun list(): Array<String>? {
        if (isRoot()) {
            return storages.toTypedArray()
        }
        val file = File(this.path)
        if (!file.exists() || !file.isDirectory || !file.canRead()) {
            return null
        }
        return sort(filter((file.list())))
    }

    private fun isStorageRoot(path: String): Boolean {
        storages.forEach { if (TextUtils.equals(it, path)) return true }
        return false
    }

    override fun back(): Array<String>? {
        val parent = File(this.path).parentFile
        if (isStorageRoot(this.path)) {
            this.path = "/"
            return list()
        }
        if (null == parent || !parent.exists() || !parent.canRead()) {
            return null
        }
        this.path = parent.path
        return list()
    }

    override fun copy(src: String, dest: String, notify: (progress: Float, index: Int, curName: String) -> Unit): Boolean {
        if (isChildManager) {
            return false
        }
        val file = File(src)
        val target = File(dest)
        if (!file.exists() || !file.canRead() || !target.canWrite()) {
            return false
        }
        file.copy(File(target, File(src).name)) { curFile ->
            notify(0f, 0, curFile.fileName() ?: "")
        }
        return true
    }

    override fun move(src: String, dest: String, notify: (progress: Float, index: Int, curName: String) -> Unit): Boolean {
        if (isChildManager) {
            return false
        }
        val file = File(src)
        val target = File(dest)
        if (!file.exists() || !file.canRead() || !target.canWrite()) {
            return false
        }
        file.move(File(target, File(src).name)) { curFile ->
            notify(0f, 0, curFile.fileName() ?: "")
        }
        return true
    }

    override fun delete(file: String, notify: (progress: Float, index: Int, curName: String) -> Unit): Boolean {
        if (isChildManager) {
            return false
        }
        val target = File(file)
        if (!target.exists() || !target.canWrite()) {
            return false
        }
        target.delete { curFile ->
            notify(0f, 0, curFile.fileName() ?: "")
        }
        return true
    }

    override fun isRoot(): Boolean {
        return TextUtils.equals("/", this.path)
    }

    @Synchronized
    override fun close() {
        this.path = "/"
    }

    companion object {
        fun sort(list: Array<String>, sort: Setting.Sort): Array<String> {
            if (list.isEmpty()) {
                return list
            }
            list.sortWith(Comparator { o1, o2 ->
                return@Comparator if (Setting.Sort.ASC == sort) {
                    o1.compareTo(o2)
                } else {
                    o2.compareTo(o1)
                }
            })
            return list
        }

        fun filter(list: Array<String>, showHide: Boolean): Array<String> {
            if (list.isEmpty() || showHide) {
                return list
            }
            return list.filter { !it.startsWith(".") }.toTypedArray()
        }
    }

    override fun getCurrentPath(): String = path

    override fun isDirectory(name: String): Boolean = File(path, name).isDirectory
}