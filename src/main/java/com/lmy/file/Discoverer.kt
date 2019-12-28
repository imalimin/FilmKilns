package com.lmy.file

import com.lmy.common.ext.fileCount
import java.io.File
import java.util.concurrent.CopyOnWriteArrayList

interface Discoverer : FileManager {
    fun of(): Discoverer
    fun open(path: String, result: (files: Array<String>?) -> Unit)
    fun list(result: (files: Array<String>?) -> Unit)
    fun back(result: (files: Array<String>?) -> Unit)
    fun copySyn(src: String, dest: String, notify: (progress: Float, index: Int, curName: String) -> Unit)
    fun moveSyn(src: String, dest: String, notify: (progress: Float, index: Int, curName: String) -> Unit)
    fun deleteSyn(file: String, notify: (progress: Float, index: Int, curName: String) -> Unit)
    fun watch(file: String, notify: (progress: Float, index: Int, curName: String) -> Unit)
    fun unwatch(file: String, notify: (progress: Float, index: Int, curName: String) -> Unit)

    class Watcher(private val manager: FileManager,
                  private val src: String,
                  private val dest: String,
                  private val type: OperateType) {
        private val listeners = CopyOnWriteArrayList<(progress: Float, index: Int, curName: String) -> Unit>()
        private var total = 0
        private var count = 0


        init {
            total = File(src).fileCount()
        }

        fun run() {
            listeners.forEach {
                it(0f, 0, "")
            }
            val ret = when (type) {
                OperateType.COPY ->
                    manager.copy(src, dest) { progress, index, curName ->
                        ++count
                        listeners.forEach { it(count / total.toFloat(), count, curName) }
                    }
                OperateType.MOVE ->
                    manager.move(src, dest) { progress, index, curName ->
                        ++count
                        listeners.forEach { it(count / total.toFloat(), count, curName) }
                    }
                OperateType.DELETE ->
                    manager.delete(src) { progress, index, curName ->
                        ++count
                        listeners.forEach {
                            it(count / total.toFloat(), count, curName)
                        }
                    }
            }
            if (ret) {
                listeners.forEach { it(1f, 0, "Success") }
            } else {
                listeners.forEach { it(-1f, 0, "Failed") }
            }
        }

        fun watch(notify: (progress: Float, index: Int, curName: String) -> Unit) {
            listeners.add(notify)
        }

        fun unwatch(notify: (progress: Float, index: Int, curName: String) -> Unit) {
            listeners.remove(notify)
        }

        companion object {
            fun copy(manager: FileManager, src: String, dest: String): Watcher {
                return Watcher(manager, src, dest, OperateType.COPY)
            }

            fun move(manager: FileManager, src: String, dest: String): Watcher {
                return Watcher(manager, src, dest, OperateType.MOVE)
            }

            fun delete(manager: FileManager, file: String): Watcher {
                return Watcher(manager, file, "", OperateType.DELETE)
            }
        }
    }

    enum class OperateType {
        COPY, MOVE, DELETE
    }
}