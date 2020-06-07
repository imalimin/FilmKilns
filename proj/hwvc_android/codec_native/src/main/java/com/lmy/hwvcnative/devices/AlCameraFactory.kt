package com.lmy.hwvcnative.devices

import android.graphics.SurfaceTexture
import android.hardware.camera2.CameraManager

object AlCameraFactory {
    fun create(manager: CameraManager?,
               index: AlAbsCamera.CameraIndex, width: Int, height: Int, tex: Int,
               onFrameAvailableListener: SurfaceTexture.OnFrameAvailableListener): AlAbsCamera {
        return AlCamera2.open(manager, index, width, height, tex, onFrameAvailableListener)
    }
}