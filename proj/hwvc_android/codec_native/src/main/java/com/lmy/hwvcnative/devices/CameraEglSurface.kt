/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */
package com.lmy.hwvcnative.devices

import android.annotation.SuppressLint
import android.graphics.SurfaceTexture
import android.opengl.GLES11Ext
import android.util.Log


/**
 * Created by lmyooyo@gmail.com on 2018/3/26.
 */
class CameraEglSurface private constructor(private val width: Int,
                                           private val height: Int) : EglOutputSurface() {
    override val name = "Camera"

    fun updateTexture() {
        createEgl(null, null)
        makeCurrent()
        createTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES)
        if (null == texture) {
            texture = CameraTexture(width, height, textureId!!).apply {
                name = "CameraTexture"
            }
        }
        initTexture()
    }

    @SuppressLint("Recycle")
    private fun initTexture() {
        if (null != surface) {
            surface?.release()
            surface = null
        }
        if (null != textureId)
            surface = SurfaceTexture(textureId!![0])
        Log.e(name, "camera textureId: ${textureId!![0]}")
    }

    override fun draw(transformMatrix: FloatArray?) {
        if (null == texture) {
            Log.e(name, "Render failed. Texture is null")
            return
        }
        texture?.draw(transformMatrix)
    }

    override fun updateLocation(vw: Int, vh: Int, ww: Int, wh: Int) {
        (texture as CameraTexture).updateFrameBuffer(width, height)
        val location = FloatArray(8)
        val textureLocation = FloatArray(8)
        calculateBestLocation(vw, vh, ww, wh, location, textureLocation)
        texture?.updateLocation(textureLocation, location)
    }

    private fun isHorizontal(): Boolean = true

    private fun calculateBestLocation(vw: Int, vh: Int, ww: Int, wh: Int,
                                      location: FloatArray, textureLocation: FloatArray) {
        val previewWidth = if (isHorizontal()) ww else wh
        val previewHeight = if (isHorizontal()) wh else ww
        val previewScale = previewWidth / previewHeight.toFloat()
        val videoScale = vw / vh.toFloat()
        var destPreviewWidth = previewWidth
        var destPreviewHeight = previewHeight
        /**
         * if (previewScale > videoScale) previewHeight不变，以previewHeight为准计算previewWidth
         * else previewWidth不变，以previewWidth为准计算previewHeight
         */
        if (previewScale > videoScale) {
            destPreviewWidth = (previewHeight * videoScale).toInt()
            if (0 != destPreviewWidth % 2) ++destPreviewWidth
        } else {
            destPreviewHeight = (previewWidth / videoScale).toInt()
            if (0 != destPreviewHeight % 2) ++destPreviewHeight
        }
        val left = (previewWidth - destPreviewWidth) / 2f / previewWidth.toFloat()
        val right = 1f - left
        val bottom = (previewHeight - destPreviewHeight) / 2f / previewHeight.toFloat()
        val top = 1 - bottom
        //顶点位置
        System.arraycopy(floatArrayOf(-1f, -1f, //LEFT,BOTTOM
                1f, -1f, //RIGHT,BOTTOM
                -1f, 1f, //LEFT,TOP
                1f, 1f//RIGHT,TOP
        ), 0, location, 0, 8)
        //顶点纹理坐标
        System.arraycopy(floatArrayOf(left, bottom, //LEFT,BOTTOM
                right, bottom, //RIGHT,BOTTOM
                left, top, //LEFT,TOP
                right, top//RIGHT,TOP
        ), 0, textureLocation, 0, 8)
    }

    private fun checkTexture() {
        if (null != texture && texture is BaseFrameBufferTexture) return
        throw RuntimeException("CameraEglSurface`s texture must be BaseFrameBufferTexture and texture must not be null")
    }

    fun getFrameBuffer(): IntArray {
        checkTexture()
        return (texture as BaseFrameBufferTexture).frameBuffer
    }

    fun getFrameBufferTexture(): IntArray {
        checkTexture()
        return (texture as BaseFrameBufferTexture).frameBufferTexture
    }

    companion object {
        fun create(width: Int, height: Int): EglOutputSurface = CameraEglSurface(width, height)
    }
}