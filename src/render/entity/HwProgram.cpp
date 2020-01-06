/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwProgram.h"
#include <vector>
#include <GLES2/gl2.h>
#include "Logcat.h"
#include "../include/Egl.h"

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

HwProgram::HwProgram(string *vertex, string *fragment) : Object() {
    this->vbo = createVBOs();
    this->program = createProgram(vertex, fragment);
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
    if (GL_NONE != program) {
        glDeleteProgram(program);
        program = GL_NONE;
    }
}

void HwProgram::draw(HwAbsTexture *tex) {
    bind();
    glActiveTexture(GL_TEXTURE0);
    tex->bind();
    glUniform1i(uTextureLocation, 0);
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

uint32_t HwProgram::createShader(uint32_t type, string *shader) {
    GLuint shaderId = glCreateShader(type);
    if (shaderId == 0) {
        Logcat::e(TAG, "%s(%d) Create Shader Failed: %d", __FUNCTION__, __LINE__, glGetError());
        return 0;
    }
    //加载Shader代码
    const char *s = shader->c_str();
    glShaderSource(shaderId, 1, &s, 0);
    //编译Shader
    glCompileShader(shaderId);
    GLint status;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
    if (GL_TRUE != status) {
#ifdef GL_DEBUG
        GLint len;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &len);
        vector<char> log(static_cast<unsigned int>(len));
        glGetShaderInfoLog(shaderId, len, nullptr, log.data());
        string str(begin(log), end(log));
        Logcat::e(TAG, "%s(%d) createShader(%d) error:%s >>>>>>>>> Source: %s",
                  __FUNCTION__, __LINE__,
                  type,
                  str.c_str(),
                  shader->c_str());
#endif
        glDeleteShader(shaderId);
        shaderId = GL_NONE;
    }
    return shaderId;
}

uint32_t HwProgram::createProgram(string *vertex, string *fragment) {
    GLuint program = glCreateProgram();
    if (program == GL_NONE) {
        Logcat::e(TAG, "%s(%d) Create program failed: %d", __FUNCTION__, __LINE__, glGetError());
        return GL_NONE;
    }
    GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertex);
    GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragment);
    if (GL_NONE == vertexShader || GL_NONE == fragmentShader) {
        return GL_NONE;
    }
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    bind();
    return program;
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

void HwProgram::bind() {
    glUseProgram(program);
}

void HwProgram::unbind() {
    glUseProgram(GL_NONE);
}

void HwProgram::setUniform1f(int32_t location, float value) {
    glUniform1f(location, value);
}

void HwProgram::setUniform2fv(int32_t location, AlVec2 &vec2) {
    float *value = new float[2]{vec2.x, vec2.y};
    glUniform2fv(location, 1, value);
    delete[] value;
}

void HwProgram::setUniformMatrix4fv(int32_t location, float *value) {
    glUniformMatrix4fv(location, 1, GL_FALSE, value);
}

void HwProgram::setUniform1i(int32_t location, int32_t value) {
    glUniform1i(location, value);
}

int32_t HwProgram::getAttribLocation(string name) {
    return glGetAttribLocation(program, name.c_str());
}

int32_t HwProgram::getUniformLocation(string name) {
    return glGetUniformLocation(program, name.c_str());
}

void HwProgram::updateMatrix(AlMatrix *matrix) {
    this->matrix = *matrix;
}