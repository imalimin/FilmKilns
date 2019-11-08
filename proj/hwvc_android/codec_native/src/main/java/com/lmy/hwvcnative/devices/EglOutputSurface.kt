/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
package com.lmy.hwvcnative.devices

import android.graphics.SurfaceTexture

abstract class EglOutputSurface : EglSurface<SurfaceTexture>() {
    fun updateTexImage() {
        surface?.updateTexImage()
    }

    fun getTransformMatrix(mtx: FloatArray) {
        surface?.getTransformMatrix(mtx)
    }

    fun getTimestamp(): Long {
        if (null != surface) {
            return surface!!.timestamp
        }
        return 0
    }
}