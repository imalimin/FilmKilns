package com.lmy.file.impl

import com.lmy.common.utils.debug_e
import com.lmy.file.Discoverer
import com.lmy.file.FileManager
import com.lmy.file.entity.Setting
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.launch

class LocalDiscoverer internal constructor(isChild: Boolean = false) : Discoverer {
    private val job = Job()
    private val scope = CoroutineScope(Dispatchers.Default + job)

    override fun of(): Discoverer = LocalDiscoverer(true)

    companion object {
        val instance: Discoverer by lazy { LocalDiscoverer() }
    }

    private val manager: FileManager = LocalFileManager(isChild)
    private val taskMap = HashMap<String, Discoverer.Watcher>()
    override fun open(path: String, result: (files: Array<String>?) -> Unit) {
        scope.launch {
            result(open(path))
        }
    }

    override fun setSetting(setting: Setting) {
        manager.setSetting(setting)
    }

    override fun of(path: String): FileManager = manager.of(path)

    override fun open(path: String): Array<String>? = manager.open(path)

    override fun list(result: (files: Array<String>?) -> Unit) {
        scope.launch {
            result(list())
        }
    }

    override fun list(): Array<String>? = manager.list()

    override fun back(result: (files: Array<String>?) -> Unit) {
        scope.launch {
            result(back())
        }
    }

    override fun getSetting(): Setting = manager.getSetting()

    override fun back(): Array<String>? = manager.back()

    override fun copy(src: String,
                      dest: String,
                      notify: (progress: Float, index: Int, curName: String) -> Unit)
            : Boolean = manager.copy(src, dest, notify)

    override fun move(src: String,
                      dest: String,
                      notify: (progress: Float, index: Int, curName: String) -> Unit)
            : Boolean = manager.move(src, dest, notify)

    override fun delete(file: String,
                        notify: (progress: Float, index: Int, curName: String) -> Unit)
            : Boolean = manager.delete(file, notify)

    override fun isRoot(): Boolean = manager.isRoot()

    override fun close() {
        manager.close()
    }

    override fun copySyn(src: String, dest: String, notify: (progress: Float, index: Int, curName: String) -> Unit) {
        if (taskMap.containsKey(src)) {
            notify(-2f, 0, "In the operation")
            return
        }
        val task = Discoverer.Watcher.copy(manager, src, dest).apply { watch(notify) }
        taskMap[src] = task
        debug_e("copySyn")
        scope.launch {
            task.run()
            task.unwatch(notify)
            taskMap.remove(src)
        }
    }

    override fun moveSyn(src: String, dest: String, notify: (progress: Float, index: Int, curName: String) -> Unit) {
        if (taskMap.containsKey(src)) {
            notify(-2f, 0, "In the operation")
            return
        }
        val task = Discoverer.Watcher.move(manager, src, dest).apply { watch(notify) }
        taskMap[src] = task
        scope.launch {
            task.run()
            task.unwatch(notify)
            taskMap.remove(src)
        }
    }

    override fun deleteSyn(file: String, notify: (progress: Float, index: Int, curName: String) -> Unit) {
        if (taskMap.containsKey(file)) {
            notify(-2f, 0, "In the operation")
            return
        }
        val task = Discoverer.Watcher.delete(manager, file).apply { watch(notify) }
        taskMap[file] = task
        debug_e("deleteSyn")
        scope.launch {
            task.run()
            task.unwatch(notify)
            taskMap.remove(file)
        }
    }

    override fun watch(file: String, notify: (progress: Float, index: Int, curName: String) -> Unit) {
        taskMap[file]?.watch(notify)
    }

    override fun unwatch(file: String, notify: (progress: Float, index: Int, curName: String) -> Unit) {
        taskMap[file]?.unwatch(notify)
    }

    override fun getCurrentPath(): String = manager.getCurrentPath()

    override fun isDirectory(name: String): Boolean = manager.isDirectory(name)
}