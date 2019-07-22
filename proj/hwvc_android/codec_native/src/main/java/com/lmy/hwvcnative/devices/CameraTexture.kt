/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */
package com.lmy.hwvcnative.devices

import android.opengl.GLES11Ext
import android.opengl.GLES20
import java.io.BufferedReader
import java.io.FileInputStream
import java.io.InputStreamReader

/**
 * Created by lmyooyo@gmail.com on 2018/3/29.
 */
class CameraTexture(width: Int, height: Int,
                    textureId: IntArray) : BaseFrameBufferTexture(width, height, textureId) {

    private var aPositionLocation = 0
    private var uTextureLocation = 0
    private var aTextureCoordinateLocation = 0
    private var uTextureMatrix = 0

    init {
        name = "CameraTexture"
        if (createProgram()) {
            init()
            initFrameBuffer()
        }
    }

    private fun readFileString(path: String): String? {
        try {
            val reader = BufferedReader(InputStreamReader(FileInputStream(path), "UTF-8"))
            val buffer = StringBuffer()
            var str: String?
            str = reader.readLine()
            while (str != null) {
                buffer.append(str)
                buffer.append("\n")
                str = reader.readLine()
            }
            return buffer.toString()
        } catch (e: Exception) {
            RuntimeException(e.toString())
        }
        return null
    }

    private fun createProgram(): Boolean {
        val vertex = VERTEX
        val fragment = FRAGMENT
        if (null == vertex || null == fragment) {
            return false
        }
        shaderProgram = createProgram(vertex!!, fragment!!)
        aPositionLocation = getAttribLocation("aPosition")
        uTextureLocation = getUniformLocation("uTexture")
        aTextureCoordinateLocation = getAttribLocation("aTextureCoord")
        uTextureMatrix = getUniformLocation("uTextureMatrix")
        return true
    }

    override fun draw(transformMatrix: FloatArray?) {
        if (null == transformMatrix)
            throw RuntimeException("TransformMatrix can not be null")
        GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, frameBuffer[0])
        GLES20.glUseProgram(shaderProgram!!)
        GLES20.glActiveTexture(GLES20.GL_TEXTURE0)
        GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, textureId[0])
        GLES20.glUniform1i(uTextureLocation, 0)
        enableVertex(aPositionLocation, aTextureCoordinateLocation)
        GLES20.glUniformMatrix4fv(uTextureMatrix, 1, false, transformMatrix, 0)

        drawer.draw()

        GLES20.glDisableVertexAttribArray(aPositionLocation)
        GLES20.glDisableVertexAttribArray(aTextureCoordinateLocation)
        GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_NONE)
        GLES20.glUseProgram(GLES20.GL_NONE)
        GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, GLES20.GL_NONE)
        GLES20.glFlush()
    }

    companion object {
        val FRAGMENT = "#extension GL_OES_EGL_image_external : require\n" +
                "precision mediump float;\n" +
                "varying mediump vec2 vTextureCoord;\n" +
                "uniform samplerExternalOES uTexture;\n" +
                "void main(){\n" +
                "    vec4 color = vec4(texture2D(uTexture, vTextureCoord).rgb, 1.0);\n" +
                "    gl_FragColor = color;\n" +
                "}"
        val VERTEX = "attribute vec4 aPosition;\n" +
                "attribute vec4 aTextureCoord;\n" +
                "uniform mat4 uTextureMatrix;\n" +
                "varying vec2 vTextureCoord;\n" +
                "void main(){\n" +
                "    gl_Position= aPosition;\n" +
                "    vTextureCoord = (uTextureMatrix * aTextureCoord).xy;\n" +
                "}"
    }
}