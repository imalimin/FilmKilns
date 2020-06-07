package com.lmy.hwvcnative.devices

import android.annotation.SuppressLint
import android.graphics.ImageFormat
import android.graphics.Point
import android.graphics.SurfaceTexture
import android.hardware.camera2.*
import java.lang.RuntimeException
import android.os.Handler
import android.os.Looper
import android.util.Log
import android.view.Surface


class AlCamera2(
    private val manager: CameraManager?,
    private val index: AlAbsCamera.CameraIndex,
    private val reqWidth: Int,
    private val reqHeight: Int,
    private val tex: Int,
    private var onFrameAvailableListener: SurfaceTexture.OnFrameAvailableListener
) : AlAbsCamera {
    private var device: CameraDevice? = null
    private var session: CameraCaptureSession? = null
    private val handler = Handler(Looper.getMainLooper())
    private var surfaceTexture: SurfaceTexture? = null
    private var surface: Surface? = null
    private val transformMatrix: FloatArray = FloatArray(16)
    private val cameraSize = Point()
    private val mIndexMap = hashMapOf<AlAbsCamera.CameraIndex, String>()
    private val callback = object : CameraDevice.StateCallback() {
        override fun onOpened(camera: CameraDevice) {
            Log.i(TAG, "onOpened")
            this@AlCamera2.device = camera
            openSession()
        }

        override fun onDisconnected(camera: CameraDevice) {
            Log.i(TAG, "onDisconnected")
            this@AlCamera2.device = null
        }

        override fun onError(camera: CameraDevice, error: Int) {
            Log.e(TAG, "onDisconnected")
            this@AlCamera2.device = null
        }
    }

    init {
        surfaceTexture = SurfaceTexture(tex)
        surface = Surface(surfaceTexture)
        surfaceTexture?.setOnFrameAvailableListener(onFrameAvailableListener)
        try {
            manager?.cameraIdList?.forEach {
                val characteristics = manager.getCameraCharacteristics(it)
                val idx = when (characteristics.get(CameraCharacteristics.LENS_FACING)) {
                    CameraCharacteristics.LENS_FACING_FRONT -> AlAbsCamera.CameraIndex.FRONT
                    CameraCharacteristics.LENS_FACING_BACK -> AlAbsCamera.CameraIndex.BACK
                    else -> return@forEach
                }
                mIndexMap[idx] = it
            }
            open()
        } catch (e: Exception) {
            e.printStackTrace()
        }
    }

    @Throws
    @SuppressLint("MissingPermission")
    private fun open() {
        if (mIndexMap.containsKey(index)) {
            setupPreviewSize()
            manager?.openCamera(mIndexMap[index]!!, callback, handler)
        }
    }

    private fun setupPreviewSize() {
        val characteristics = manager?.getCameraCharacteristics(mIndexMap[index]!!)
        val map = characteristics?.get(CameraCharacteristics.SCALER_STREAM_CONFIGURATION_MAP)
        val sizes = map?.getOutputSizes(ImageFormat.JPEG)
        if (null != sizes) {
            val size = AlAbsCamera.chooseBestSize(sizes, reqWidth, reqHeight)
            cameraSize.x = size.width
            cameraSize.y = size.height
            surfaceTexture?.setDefaultBufferSize(cameraSize.x, cameraSize.y)
            Log.i(TAG, "setupPreviewSize ${cameraSize.x}x${cameraSize.y}")
        }
    }

    private fun openSession() {
        try {
            if (null == surface) {
                throw RuntimeException("Surface is null.")
            }
            device?.createCaptureSession(
                arrayListOf(surface!!),
                object : CameraCaptureSession.StateCallback() {
                    override fun onConfigureFailed(session: CameraCaptureSession) {
                        this@AlCamera2.session = null
                        Log.e(TAG, "onConfigureFailed")
                    }

                    override fun onConfigured(session: CameraCaptureSession) {
                        this@AlCamera2.session = session
                        startPreview()
                    }
                }, handler
            )
        } catch (e: Exception) {
            e.printStackTrace()
        }
    }

    private fun startPreview() {
        val builder = device?.createCaptureRequest(CameraDevice.TEMPLATE_PREVIEW)
        builder?.addTarget(surface!!)
        builder?.set(
            CaptureRequest.CONTROL_AF_MODE,
            CaptureRequest.CONTROL_AF_MODE_CONTINUOUS_VIDEO
        )
        if (null != builder) {
            session?.setRepeatingRequest(
                builder.build(),
                null, handler
            )
        }
    }

    override fun getCameraSize(): Point {
        return cameraSize
    }

    override fun switchCamera(index: AlAbsCamera.CameraIndex) {
    }

    override fun release() {
        Log.i(TAG, "release")
        session?.close()
        device?.close()
        surface?.release()
        surfaceTexture?.release()
    }

    override fun draw() {
        surfaceTexture?.updateTexImage()
        surfaceTexture?.getTransformMatrix(transformMatrix)
    }

    override fun getMatrix(): FloatArray = transformMatrix

    override fun timestamp(): Long {
        return if (null != surfaceTexture) surfaceTexture!!.timestamp else 0L
    }

    companion object {
        private const val TAG = "AlCamera2"
        fun open(
            manager: CameraManager?,
            index: AlAbsCamera.CameraIndex,
            width: Int,
            height: Int,
            tex: Int,
            onFrameAvailableListener: SurfaceTexture.OnFrameAvailableListener
        ): AlAbsCamera {
            if (width <= 0 || width <= 0) {
                throw RuntimeException("Preview size must greater than 0")
            }
            return AlCamera2(manager, index, width, height, tex, onFrameAvailableListener)
        }
    }
}