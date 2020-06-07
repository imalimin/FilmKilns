/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
package com.lmy.hwvcnative.devices

import android.graphics.Point
import android.graphics.SurfaceTexture
import android.hardware.Camera
import android.hardware.camera2.CameraManager
import android.util.Log
import java.lang.RuntimeException

/**
 * Created by aliminabc@gmail.com on 2018/3/21.
 */
class CameraWrapper private constructor(
    index: AlAbsCamera.CameraIndex,
    private val reqWidth: Int,
    private val reqHeight: Int,
    private val tex: Int,
    private var onFrameAvailableListener: SurfaceTexture.OnFrameAvailableListener
) : AlAbsCamera {
    companion object {
        private val PREPARE = 0x1
        const val TAG = "CameraWrapper"
        fun open(
            manager: CameraManager?,
            index: AlAbsCamera.CameraIndex, width: Int, height: Int, tex: Int,
            onFrameAvailableListener: SurfaceTexture.OnFrameAvailableListener
        ): AlAbsCamera {
            if (width <= 0 || width <= 0) {
                throw RuntimeException("Preview size must greater than 0")
            }
            return CameraWrapper(index, width, height, tex, onFrameAvailableListener)
        }
    }

    private var mCamera: Camera? = null
    private var mCameras = 0
    private var mCameraIndex: AlAbsCamera.CameraIndex? = null
    private val surface: SurfaceTexture
    private var transformMatrix: FloatArray = FloatArray(16)
    var cameraWidth = 0
    var cameraHeight = 0

    init {
        mCameras = CameraHelper.getNumberOfCameras()
        surface = SurfaceTexture(tex)
        openCamera(index)
    }

    override fun switchCamera(index: AlAbsCamera.CameraIndex) {
        openCamera(index)
    }

    override fun getCameraSize(): Point {
        return Point(cameraWidth, cameraHeight)
    }

    private fun openCamera(index: AlAbsCamera.CameraIndex) {
        val tmp =
            if (index == AlAbsCamera.CameraIndex.FRONT && mCameras < 2) {//如果没有前置摄像头，则强制使用后置摄像头
                AlAbsCamera.CameraIndex.BACK
            } else {
                index
            }
        if (null != mCameraIndex && mCameraIndex == tmp) //如果已经打开过摄像头，并且当前已经是index，则不做改变
            return
        mCameraIndex = tmp
//        GLEventPipeline.INSTANCE.queueEvent(Runnable {
        stopPreview()
        updateTexture()
        if (prepare()) {
            startPreview()
        }
//        })
    }

    private fun updateTexture() {
        surface.setOnFrameAvailableListener(onFrameAvailableListener)
    }

    private fun getCameraIndex(): Int {
        if (mCameraIndex == AlAbsCamera.CameraIndex.FRONT)
            return Camera.CameraInfo.CAMERA_FACING_FRONT
        return Camera.CameraInfo.CAMERA_FACING_BACK
    }

    private fun prepare(): Boolean {
        if (0 == mCameras) {
            Log.e(TAG, "Unavailable camera")
            return false
        }

        val time = System.currentTimeMillis()
        mCamera = openCamera(getCameraIndex())
        Log.i(TAG, "open time: ${System.currentTimeMillis() - time}")
        if (null == mCamera) {
            Log.e(TAG, "mCamera is null!")
            return false
        }
        val cameraParam = mCamera!!.parameters
        CameraHelper.setPreviewSize(cameraParam, reqWidth, reqHeight)
        CameraHelper.setColorFormat(cameraParam)
        CameraHelper.setFocusMode(cameraParam)
        CameraHelper.setFps(cameraParam, 30)
        CameraHelper.setAutoExposureLock(cameraParam, false)
        CameraHelper.setSceneMode(cameraParam, Camera.Parameters.SCENE_MODE_AUTO)
        CameraHelper.setFlashMode(cameraParam, Camera.Parameters.FLASH_MODE_OFF)
        CameraHelper.setAntibanding(cameraParam, Camera.Parameters.ANTIBANDING_AUTO)
        CameraHelper.setVideoStabilization(cameraParam, true)
        val size = cameraParam.previewSize
        cameraWidth = size.height
        cameraHeight = size.width
        val fps = IntArray(2)
        cameraParam.getPreviewFpsRange(fps)
        Log.i(
            TAG, "Camera config: Size(${cameraWidth}x$cameraHeight\n" +
                    "Format(${cameraParam.previewFormat})\n" +
                    "FocusMode(${cameraParam.focusMode})\n" +
                    "Fps(${fps[0]}-${fps[1]})\n" +
                    "AutoExposureLock(${cameraParam.autoExposureLock})\n" +
                    "SceneMode(${cameraParam.sceneMode})\n" +
                    "FlashMode(${cameraParam.flashMode})\n" +
                    "Antibanding(${cameraParam.antibanding})\n" +
                    "VideoStabilization(${cameraParam.videoStabilization})"
        )
        try {
            mCamera!!.parameters = cameraParam
            Log.i(TAG, "Camera config")
        } catch (e: Exception) {
            mCamera?.release()
            Log.i(TAG, "Camera $mCameraIndex open failed. Please check parameters")
            e.printStackTrace()
            return false
        }
        return true
    }

    private fun openCamera(index: Int): Camera? {
        return try {
            val camera = Camera.open(index)
            camera.setDisplayOrientation(90)
            camera
        } catch (e: SecurityException) {
            Log.i(TAG, "Camera $index open failed, No permission")
            e.printStackTrace()
            null
        } catch (e: Exception) {
            Log.i(TAG, "Camera $index open failed")
            e.printStackTrace()
            null
        }
    }

    //Run on egl thread
    override fun release() {
//        GLEventPipeline.INSTANCE.queueEvent(Runnable {
        stopPreview()
        releaseTexture()
//        })
    }

    private fun startPreview() {
        if (null == mCamera) {
            Log.i(TAG, "Start preview failed, mCamera is null")
            return
        }
        try {
            mCamera!!.setPreviewTexture(surface)
            mCamera!!.startPreview()
        } catch (e: Exception) {
            release()
            Log.e(TAG, "Start preview failed")
            e.printStackTrace()
        }
    }

    private fun stopPreview() {
        if (null == mCamera) return
        try {
            mCamera!!.setPreviewTexture(null)
            mCamera!!.setPreviewCallback(null)
            mCamera!!.stopPreview()
            mCamera!!.release()
            mCamera = null
        } catch (e: Exception) {
            Log.e(TAG, "Stop preview failed")
            e.printStackTrace()
        }
    }

    private fun releaseTexture() {
        surface.release()
        Log.i(TAG, "releaseTexture")
    }

    override fun draw() {
        surface.updateTexImage()
        surface.getTransformMatrix(transformMatrix)
    }

    override fun getMatrix(): FloatArray = transformMatrix

    /**
     * @return nanoseconds
     */
    override fun timestamp(): Long = surface.timestamp
}