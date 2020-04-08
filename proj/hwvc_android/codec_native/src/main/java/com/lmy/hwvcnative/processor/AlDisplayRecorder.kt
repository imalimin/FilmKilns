package com.lmy.hwvcnative.processor

import android.graphics.Bitmap
import android.graphics.SurfaceTexture
import android.hardware.display.DisplayManager
import android.hardware.display.VirtualDisplay
import android.media.ImageReader
import android.media.projection.MediaProjection
import android.os.Handler
import android.os.Looper
import android.util.Log
import android.view.Surface
import com.lmy.hwvcnative.CPPObject
import java.io.FileOutputStream
import java.nio.ByteBuffer

class AlDisplayRecorder(private val mp: MediaProjection,
                        private val dw: Int,
                        private val dh: Int,
                        private val dpi: Int) : CPPObject(), SurfaceTexture.OnFrameAvailableListener {
    private val mHandler = Handler(Looper.getMainLooper())
    private var vd: AlVirtualDisplay? = null
    private var onRecordProgressListener: ((Long) -> Unit)? = null

    init {
        handler = create()
    }

    fun setOutputFilePath(filePath: String) {
        if (0L == handler) return
        setOutputFilePath(handler, filePath)
    }

    fun setFormat(width: Int, height: Int, sampleFormat: Int = 102,
                  channels: Int = 2, sampleRate: Int = 44100) {
        if (0L == handler) return
        setFormat(handler, width, height, sampleFormat, channels, sampleRate)
    }

    fun start() {
        if (0L == handler) return
        start(handler)
    }

    fun pause() {
        if (0L == handler) return
        pause(handler)
    }

    fun release() {
        if (0L == handler) return
        postEvent(handler, EVENT_RELEASE)
        release(handler)
        handler = 0L
    }

    override fun onFrameAvailable(surfaceTexture: SurfaceTexture) {
        if (0L == handler) return
        postEvent(handler, EVENT_DRAW)
    }

    override fun onNativeMessage(what: Int, arg1: Int) {
        when (what) {
            HwCameraRecorder.EVENT_RELEASE -> {
                vd?.release()
                Log.i("AlDisplayRecorder", "release")
            }
            HwCameraRecorder.EVENT_DRAW -> {
                vd?.draw()
                val time = vd!!.timestamp()
                Log.i("AlDisplayRecorder", "draw $time")
                if (0L != handler) {
                    invalidate(handler, vd!!.matrix(), time, dw, dh)
                }
            }
        }
    }

    fun onNativePrepared(oesTex: Int) {
        if (oesTex <= 0) {
            return
        }
        Log.i("AlDisplayRecorder", "OES tex $oesTex")
        vd = AlVirtualDisplay.open(mp, dw, dh, dpi, oesTex)
        vd?.setOnFrameAvailableListener(this)
    }

    fun onRecordProgress(timeInUs: Long) {
        mHandler.post {
            onRecordProgressListener?.invoke(timeInUs)
        }
    }

    fun setOnRecordProgressListener(listener: (Long) -> Unit) {
        this.onRecordProgressListener = listener
    }

    fun showAt(surface: Surface) {
        if (isNativeNull()) return
        Log.i("AlDisplayRecorder", "showAt")
        showAt(handler, surface)
    }

    private external fun create(): Long
    private external fun showAt(handler: Long, surface: Surface)
    private external fun start(handler: Long)
    private external fun pause(handler: Long)
    private external fun release(handler: Long)
    private external fun postEvent(handler: Long, what: Int)
    private external fun invalidate(handler: Long, matrix: FloatArray, tsInNs: Long,
                                    cw: Int, ch: Int)

    private external fun setOutputFilePath(handler: Long, filePath: String)
    private external fun setFormat(handler: Long, width: Int, height: Int, sampleFormat: Int,
                                   channels: Int, sampleRate: Int)

    companion object {
        const val EVENT_PREPARE = 1
        const val EVENT_RELEASE = 2
        const val EVENT_DRAW = 3
        const val EVENT_SWAP = 4
    }
}

class AlVirtualDisplay private constructor(private val mp: MediaProjection,
                                           private val dw: Int,
                                           private val dh: Int,
                                           private val dpi: Int,
                                           private val tex: Int) {
    private var vd: VirtualDisplay? = null
    private var texture: SurfaceTexture? = null
    private var transformMatrix: FloatArray = FloatArray(16)
    private var reader: ImageReader? = null
    private val callback = object : VirtualDisplay.Callback() {
        override fun onResumed() {
            Log.i("AlDisplayRecorder", "onResumed")
        }

        override fun onStopped() {
            Log.i("AlDisplayRecorder", "onStopped")
        }

        override fun onPaused() {
            Log.i("AlDisplayRecorder", "onPaused")
        }
    }

    init {
//        reader = ImageReader.newInstance(dw, dh, PixelFormat.RGBA_8888, 5)
//        reader?.setOnImageAvailableListener({ reader ->
//            save(reader)
//            reader?.close()
//        }, null)
        Log.i("AlDisplayRecorder", "setup $dw x $dh, $dpi")
        texture = SurfaceTexture(tex)
        texture?.setDefaultBufferSize(dw, dh)
        vd = mp.createVirtualDisplay("hwvc", dw, dh, dpi,
                DisplayManager.VIRTUAL_DISPLAY_FLAG_AUTO_MIRROR,
                Surface(texture), callback, Handler(Looper.getMainLooper()))
    }

    private var src: ByteArray? = null
    private var dst: ByteArray? = null
    private fun save(reader: ImageReader) {
        val image = reader.acquireNextImage()
        val width = image.width
        val height = image.height
        val planes = image.planes
        val rowStride = planes[0].rowStride
        val pixelStride = planes[0].pixelStride

        if (null == src) {
            src = ByteArray(planes[0].buffer.remaining())
        }
        if (null == dst) {
            dst = ByteArray(width * height * pixelStride)
        }
        planes[0].buffer.get(src!!)
        for (i in 0 until height) {
            System.arraycopy(src, i * rowStride, dst, i * width * pixelStride, width * pixelStride)
        }
        val bmp = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888)
        bmp.copyPixelsFromBuffer(ByteBuffer.wrap(dst))
        val fos = FileOutputStream("/sdcard/000000.jpg")
        bmp.compress(Bitmap.CompressFormat.JPEG, 80, fos)
        fos.close()
    }

    fun setOnFrameAvailableListener(l: SurfaceTexture.OnFrameAvailableListener) {
        texture?.setOnFrameAvailableListener(l)
    }

    fun draw() {
        texture?.updateTexImage()
        texture?.getTransformMatrix(transformMatrix)
    }

    fun timestamp(): Long = texture!!.timestamp

    fun matrix(): FloatArray = transformMatrix

    fun release() {
        vd?.release()
        mp.stop()
        texture?.release()
        reader?.close()
    }

    companion object {
        fun open(mp: MediaProjection, width: Int, height: Int, dpi: Int, tex: Int): AlVirtualDisplay {
            return AlVirtualDisplay(mp, width, height, dpi, tex)
        }
    }
}