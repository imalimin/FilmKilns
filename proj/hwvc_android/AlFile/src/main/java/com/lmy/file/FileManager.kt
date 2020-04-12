package com.lmy.file

import com.lmy.file.entity.Setting

interface FileManager {
    fun setSetting(setting: Setting)
    fun getSetting(): Setting
    /**
     * 构造一个允许部分操作的FileManager
     * @path 当前目录完整路径
     */
    fun of(path: String): FileManager

    /**
     * 打开一个文件夹，并切换当前目录
     * @path 当前目录名称
     * @return 所有子文件
     */
    fun open(name: String): Array<String>?

    /**
     * 列出当前目录下的所有文件
     * @return 所有子文件
     */
    fun list(): Array<String>?

    /**
     * 返回到上一层目录
     * @return 所有子文件
     */
    fun back(): Array<String>?

    /**
     * 复制文件或目录到指定目录
     * @src 需要复制的文件或文件夹，完整路径
     * @dest 目标文件夹，完整路径
     * @notify 进度通知
     * @return true成功，否则失败
     */
    fun copy(src: String, dest: String, notify: (progress: Float, index: Int, curName: String) -> Unit): Boolean

    /**
     * 移动文件或目录到指定目录
     * @src 需要复制的文件或文件夹
     * @dest 目标文件夹，完整路径
     * @notify 进度通知，完整路径
     * @return true成功，否则失败
     */
    fun move(src: String, dest: String, notify: (progress: Float, index: Int, curName: String) -> Unit): Boolean

    /**
     * 删除文件或目录
     * @file 被删除的文件或目录，完整路径
     * @notify 进度通知
     * @return true成功，否则失败
     */
    fun delete(file: String, notify: (progress: Float, index: Int, curName: String) -> Unit): Boolean

    /**
     * 判断当前目录是不是根目录
     * @return
     */
    fun isRoot(): Boolean

    /**
     * 判断当前目录下的文件是否是文件夹
     * @return
     */
    fun isDirectory(name: String): Boolean

    /**
     * 关闭操作
     */
    fun close()

    /**
     * 获取当前路径
     * @return
     */
    fun getCurrentPath(): String
}