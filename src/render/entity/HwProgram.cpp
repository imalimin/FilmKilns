/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwProgram.h"
#include <vector>
#include <GLES2/gl2.h>
#include "Logcat.h"
#include "../include/Egl.h"

HwProgram *HwProgram::create(string *vertex, string *fragment) {
    if (EGL_NO_CONTEXT == Egl::currentContext()) {
        Logcat::e("hwvc", "Please attach an egl context first.");
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
            0.0f, 0.0f,//LEFT,BOTTOM
            1.0f, 0.0f,//RIGHT,BOTTOM
            0.0f, 1.0f,//LEFT,TOP
            1.0f, 1.0f//RIGHT,TOP
    };
    // st
    float *position = new float[8]{
            -1.0f, -1.0f,//LEFT,BOTTOM
            1.0f, -1.0f,//RIGHT,BOTTOM
            -1.0f, 1.0f,//LEFT,TOP
            1.0f, 1.0f//RIGHT,TOP
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

void HwProgram::draw(HwTexture *tex) {
    glUseProgram(program);
    glActiveTexture(GL_TEXTURE0);
    tex->bind();
    glUniform1i(uTextureLocation, 0);
    enableVertex(aPositionLocation, aTextureCoordinateLocation);
    setUniformMatrix4fv(uTextureMatrix, matrix.data());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableVertexAttribArray(aPositionLocation);
    glDisableVertexAttribArray(aTextureCoordinateLocation);
    tex->unbind();
    glUseProgram(GL_NONE);
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
        Logcat::e("hwvc", "Create Shader Failed: %d", glGetError());
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
        Logcat::e("HWVC", "BaseDrawer::createShader(%d) error:%s >>>>>>>>> Source: %s",
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
        Logcat::e("hwvc", "Create program failed: %d", glGetError());
        return GL_NONE;
    }
    GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertex);
    GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragment);
    if (GL_NONE == vertexShader || GL_NONE == fragmentShader) {
        return GL_NONE;
    }
    //附着顶点和片段着色器
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    //链接program
    glLinkProgram(program);
    //告诉OpenGL ES使用此program
    glUseProgram(program);
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

void HwProgram::setUniform1f(int32_t location, float value) {
    glUniform1f(location, value);
}

void HwProgram::setUniformMatrix4fv(int32_t location, float *value) {
    glUniformMatrix4fv(location, 1, GL_FALSE, value);
}

int32_t HwProgram::getAttribLocation(string name) {
    return glGetAttribLocation(program, name.c_str());
}

int32_t HwProgram::getUniformLocation(string name) {
    return glGetUniformLocation(program, name.c_str());
}

void HwProgram::updateMatrix(HwMatrix *matrix) {
    this->matrix = *matrix;
}