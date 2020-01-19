/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "HwProgram.h"
#include <vector>
#include <GLES2/gl2.h>
#include "Logcat.h"
#include "Egl.h"

#define TAG "HwProgram"

void HwProgram::calculateFitWinVertex(float *vertex,
                                      Size texSize,
                                      Size winSize,
                                      int32_t scaleType) {
    int viewWidth = winSize.width;
    int viewHeight = winSize.height;
    float viewScale = viewWidth / (float) viewHeight;
    float picScale = texSize.width / (float) texSize.height;

    int destViewWidth = viewWidth;
    int destViewHeight = viewHeight;
    if (viewScale > picScale) {
        destViewWidth = (int) (viewHeight * picScale);
    } else {
        destViewHeight = (int) (viewWidth / picScale);
    }
    float left = -destViewWidth / (float) viewWidth;
    float right = -left;
    float bottom = -destViewHeight / (float) viewHeight;
    float top = -bottom;

    vertex[0] = left;
    vertex[1] = bottom;
    vertex[2] = right;
    vertex[3] = bottom;
    vertex[4] = left;
    vertex[5] = top;
    vertex[6] = right;
    vertex[7] = top;
}

HwProgram *HwProgram::create(string *vertex, string *fragment) {
    if (EGL_NO_CONTEXT == Egl::currentContext()) {
        Logcat::e(TAG, "%s(%d) Please attach an egl context first.", __FUNCTION__, __LINE__);
        return nullptr;
    }
    return new HwProgram(vertex, fragment);
}

HwProgram::HwProgram(string *vertex, string *fragment) : AlAbsGLProgram(vertex, fragment) {
    this->vbo = createVBOs();
    uTextureLocation = getUniformLocation("uTexture");
    uTextureMatrix = getUniformLocation("uTextureMatrix");
    aPositionLocation = getAttribLocation("aPosition");
    aTextureCoordinateLocation = getAttribLocation("aTextureCoord");
    // xy
    float *texCoordinate = new float[8]{
            0.0f, 1.0f,//LEFT,TOP
            1.0f, 1.0f,//RIGHT,TOP
            0.0f, 0.0f,//LEFT,BOTTOM
            1.0f, 0.0f,//RIGHT,BOTTOM
    };
    // st
    float *position = new float[8]{
            -1.0f, 1.0f,//LEFT,TOP
            1.0f, 1.0f,//RIGHT,TOP
            -1.0f, -1.0f,//LEFT,BOTTOM
            1.0f, -1.0f,//RIGHT,BOTTOM
    };
    updateLocation(texCoordinate, position);
    delete[]texCoordinate;
    delete[]position;
}

HwProgram::~HwProgram() {
    if (GL_NONE != vbo) {
        glDeleteBuffers(1, &vbo);
        vbo = GL_NONE;
    }
}

void HwProgram::draw(HwAbsTexture *tex) {
    bind();
    if (uTextureLocation >= 0) {
        glActiveTexture(GL_TEXTURE0);
        tex->bind();
        glUniform1i(uTextureLocation, 0);
    }
    enableVertex(aPositionLocation, aTextureCoordinateLocation);
    if (uTextureMatrix >= 0) {
        setUniformMatrix4fv(uTextureMatrix, matrix.data());
    }
    /// n = 顶点数，当0 == n % 2时，三角形为(n - 2, n - 1, n)
    /// n = 顶点数，当0 != n % 2时，三角形为(n - 1, n - 2, n)
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableVertexAttribArray(aPositionLocation);
    glDisableVertexAttribArray(aTextureCoordinateLocation);
    tex->unbind();
    unbind();
    glFlush();
}

uint32_t HwProgram::createVBOs() {
    uint32_t vbo = GL_NONE;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, HW_VERTEX_BYTE_SIZE * 2, nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    return vbo;
}

void HwProgram::updateVBOs() {
    if (!requestUpdateLocation) return;
    requestUpdateLocation = false;
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, HW_VERTEX_BYTE_SIZE, position);
    glBufferSubData(GL_ARRAY_BUFFER, HW_VERTEX_BYTE_SIZE, HW_VERTEX_BYTE_SIZE, texCoordinate);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
}

void HwProgram::enableVertex(uint32_t posLoc, uint32_t texLoc) {
    updateVBOs();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(posLoc);
    glEnableVertexAttribArray(texLoc);
    //xy
    glVertexAttribPointer(posLoc, HW_COUNT_PER_VERTEX, GL_FLOAT, GL_FALSE, 0, 0);
    //st
    glVertexAttribPointer(texLoc, HW_COUNT_PER_VERTEX, GL_FLOAT, GL_FALSE, 0,
                          reinterpret_cast<const void *>(HW_VERTEX_BYTE_SIZE));
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
}

void HwProgram::updateLocation(float *texCoordinate, float *position) {
    if (texCoordinate) {
        memcpy(this->texCoordinate, texCoordinate, (size_t) HW_VERTEX_BYTE_SIZE);
    }
    if (position) {
        memcpy(this->position, position, (size_t) HW_VERTEX_BYTE_SIZE);
    }
    requestUpdateLocation = true;
}

void HwProgram::updateMatrix(AlMatrix *matrix) {
    this->matrix = *matrix;
}
