/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
package com.lmy.hwvcnative.devices

import android.graphics.ImageFormat
import android.hardware.Camera

/**
 * Created by aliminabc@gmail.com on 2018/3/21.
 */
class CameraHelper {

    companion object {
        fun getNumberOfCameras(): Int {
            return Camera.getNumberOfCameras()
        }

        fun setPreviewSize(cameraParam: Camera.Parameters, requestWidth: Int, requestHeight: Int) {
            val supportSizes = cameraParam.supportedPreviewSizes
            var bestWidth = 0
            var bestHeight = 0
            for (size in supportSizes) {
                if (size.width >= requestHeight//预览宽大于输出宽
                        && size.height >= requestWidth//预览高大于输出高
                        && (size.width * size.height < bestWidth * bestHeight || 0 == bestWidth * bestHeight)) {//选择像素最少的分辨率
                    bestWidth = size.width
                    bestHeight = size.height
                }
            }
            if (0 == bestWidth || 0 == bestHeight) {
                throw RuntimeException("Camera invalid size($bestWidth x $bestHeight)")
            }
            cameraParam.setPreviewSize(bestWidth, bestHeight)
        }

        fun setColorFormat(cameraParam: Camera.Parameters) {
            if (cameraParam.supportedPreviewFormats.contains(ImageFormat.NV21))
                cameraParam.previewFormat = ImageFormat.NV21
        }

        fun setFocusMode(cameraParam: Camera.Parameters) {
            val modes = cameraParam.supportedFocusModes ?: return
            when {
                modes.contains(Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO) -> cameraParam.focusMode = Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO
                modes.contains(Camera.Parameters.FOCUS_MODE_AUTO) -> cameraParam.focusMode = Camera.Parameters.FOCUS_MODE_AUTO
                modes.contains(Camera.Parameters.FOCUS_MODE_FIXED) -> cameraParam.focusMode = Camera.Parameters.FOCUS_MODE_FIXED
            }
        }

        fun setFps(cameraParam: Camera.Parameters, requestFps: Int) {
            val fpsRanges = cameraParam.supportedPreviewFpsRange
            var fpsRange = IntArray(2)
            cameraParam.getPreviewFpsRange(fpsRange)
            if (requestFps > 0) {
                fpsRanges.forEach {
                    if (requestFps * 1000 >= it[0] && it[0] > fpsRange[0])
                        fpsRange = it
                }
            } else {
                fpsRanges.forEach {
                    if (it[0] != it[1] && Math.abs(it[1] - it[0]) > Math.abs(fpsRange[1] - fpsRange[0])) {
                        fpsRange = it
                    }
                }
            }
            cameraParam.setPreviewFpsRange(fpsRange[0], fpsRange[1])
        }

        fun setAutoExposureLock(cameraParam: Camera.Parameters, flag: Boolean) {
            if (cameraParam.isAutoExposureLockSupported)
                cameraParam.autoExposureLock = flag
        }

        fun setSceneMode(cameraParam: Camera.Parameters, mode: String) {
            if (cameraParam.supportedSceneModes != null && cameraParam.supportedSceneModes.contains(mode)) {
                cameraParam.sceneMode = mode
            }
        }

        fun setFlashMode(cameraParam: Camera.Parameters, mode: String) {
            if (cameraParam.supportedFlashModes != null && cameraParam.supportedFlashModes.contains(mode)) {
                cameraParam.flashMode = mode
            }
        }

        fun setAntibanding(cameraParam: Camera.Parameters, mode: String) {
            if (cameraParam.supportedAntibanding != null && cameraParam.supportedAntibanding.contains(mode)) {
                cameraParam.antibanding = mode
            }
        }

        fun setVideoStabilization(cameraParam: Camera.Parameters, flag: Boolean) {
            if (cameraParam.isVideoStabilizationSupported) {
                cameraParam.videoStabilization = flag
            }
        }
    }
}