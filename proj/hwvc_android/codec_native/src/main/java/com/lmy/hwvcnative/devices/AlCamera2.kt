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
import com.lmy.hwvcnative.devices.entity.CamMetadata
import com.lmy.hwvcnative.devices.entity.kFacing
import java.util.*
import kotlin.collections.ArrayList


class AlCamera2(
    private val manager: CameraManager?,
    private var index: AlAbsCamera.CameraIndex,
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
    private val mCameraList = ArrayList<CamMetadata>()
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
                val focalDistance =
                    characteristics.get(CameraCharacteristics.LENS_INFO_HYPERFOCAL_DISTANCE) ?: 0f
                val face = characteristics.get(CameraCharacteristics.LENS_FACING)
                Log.i(TAG, "$face")
                val facing = when (face) {
                    CameraCharacteristics.LENS_FACING_FRONT -> kFacing.FRONT
                    CameraCharacteristics.LENS_FACING_BACK -> kFacing.BACK
                    else -> return@forEach
                }
                mCameraList.add(CamMetadata(it, facing, focalDistance))
            }
            Log.i(TAG, mCameraList.toString())
            open()
        } catch (e: Exception) {
            e.printStackTrace()
        }
    }

    private fun getDefaultCamera(index: AlAbsCamera.CameraIndex): CamMetadata {
        val idx = if (AlAbsCamera.CameraIndex.FRONT == index) kFacing.FRONT else kFacing.BACK
        val list = ArrayList<CamMetadata>()
        mCameraList.forEach {
            if (idx == it.facing) {
                list.add(it)
            }
        }
        if (1 == list.size) {
            return list[0]
        }
        if (list.size > 1) {
            list.sortByDescending { it.focalDistance }
            return list[list.size / 2]
        }
        return mCameraList[0]
    }

    @Throws
    @SuppressLint("MissingPermission")
    private fun open() {
        val meta = getDefaultCamera(index)
        setupPreviewSize()
        manager?.openCamera(meta.id, callback, handler)
    }

    private fun close() {
        device?.close()
    }

    @Throws
    private fun setupPreviewSize() {
        val characteristics = manager?.getCameraCharacteristics(getDefaultCamera(index).id)
        val map = characteristics?.get(CameraCharacteristics.SCALER_STREAM_CONFIGURATION_MAP)
        var sizes = map?.getOutputSizes(ImageFormat.YUV_420_888)
        if (null != map && map.isOutputSupportedFor(ImageFormat.YUV_420_888)) {
            sizes = map.getOutputSizes(ImageFormat.JPEG)
        }
        if (null != sizes) {
            val size = AlAbsCamera.chooseBestSize(sizes, reqWidth, reqHeight)
            cameraSize.x = size.width
            cameraSize.y = size.height
            ///需要再反过来
            surfaceTexture?.setDefaultBufferSize(cameraSize.y, cameraSize.x)
            Log.i(
                TAG,
                "setupPreviewSize ${cameraSize.x}x${cameraSize.y}, ${map?.isOutputSupportedFor(
                    surface
                )}"
            )
        } else {
            throw RuntimeException("Could not find any preview size.")
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

    private fun closeSession() {
        session?.stopRepeating()
        session?.close()
    }

    private fun startPreview() {
        val builder = device?.createCaptureRequest(CameraDevice.TEMPLATE_RECORD)
        builder?.addTarget(surface!!)
        builder?.set(
            CaptureRequest.CONTROL_AF_MODE,
            CaptureRequest.CONTROL_AF_MODE_CONTINUOUS_VIDEO
        )
//        builder?.set(
//            CaptureRequest.CONTROL_VIDEO_STABILIZATION_MODE,
//            CaptureRequest.CONTROL_VIDEO_STABILIZATION_MODE_ON
//        )
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
        this.index = index
        closeSession()
        close()
        open()
    }

    override fun release() {
        Log.i(TAG, "release")
        closeSession()
        close()
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