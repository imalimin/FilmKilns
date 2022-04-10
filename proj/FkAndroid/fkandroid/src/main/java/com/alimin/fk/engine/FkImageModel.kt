package com.alimin.fk.engine

import fk_pb.FkPictureModelKt

interface FkNativeMsgListener {
    fun onNativeMsgReceived(what: Int, pbObject: ByteArray?): Boolean
}

class FkImageModel(val engine: FkImage) : FkEngine() {
    private var mSyncLock = Object()
    override fun onCreateInstance(): Long = nativeCreateInstance(engine.getHandle())
    override fun create() {
        super.create()
        if (!isNull()) {
            nativeCreate(getHandle())
        }
    }

    override fun destroy() {
        super.destroy()
        synchronized(mSyncLock) {
            if (!isNull()) {
                nativeDestroy(getHandle())
                releaseHandle()
            }
        }
    }

    override fun start() {
        super.start()
        if (!isNull()) {
            nativeStart(getHandle())
        }
    }

    override fun stop() {
        super.stop()
        if (!isNull()) {
            nativeStop(getHandle())
        }
    }

    fun save(file: String): Int {
        if (!isNull()) {
            return nativeSave(getHandle(), file)
        }
        return -1
    }

    fun load(file: String): Int {
        if (!isNull()) {
            return nativeLoad(getHandle(), file)
        }
        return -1
    }

    fun countOfLayer(): Int {
        return 0
    }

    fun getLayers(): Int {
        if (!isNull()) {
            return nativeGetLayers(getHandle(), object :FkNativeMsgListener{
                override fun onNativeMsgReceived(what: Int, pbObject: ByteArray?): Boolean {
                    when(what) {
                        0 -> {
                            if (null != pbObject) {
                                val model = FkPictureModelKt
                            }
                            return pbObject != null
                        }
                    }
                    return false
                }
            })
        }
        return -1
    }

    fun getLayer(layer: Int) {

    }

    private external fun nativeCreateInstance(imageEngineHandle: Long): Long
    private external fun nativeCreate(handle: Long)
    private external fun nativeDestroy(handle: Long)
    private external fun nativeStart(handle: Long)
    private external fun nativeStop(handle: Long)

    private external fun nativeSave(handle: Long, file: String):Int
    private external fun nativeLoad(handle: Long, file: String):Int
    private external fun nativeGetLayers(handle: Long, listener: FkNativeMsgListener):Int
}