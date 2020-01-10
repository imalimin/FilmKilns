/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlPointProgram.h"
#include "AlEgl.h"
#include "Logcat.h"

#define TAG "AlPointProgram"

AlPointProgram *AlPointProgram::create(std::string *v, std::string *f) {
    if (EGL_NO_CONTEXT == AlEgl::currentContext()) {
        Logcat::e(TAG, "%s(%d) Please attach an egl context first.", __FUNCTION__, __LINE__);
        return nullptr;
    }
    return new AlPointProgram(v, f);
}

AlPointProgram::AlPointProgram(std::string *v, std::string *f) : AlAbsGLProgram(v, f) {
    aPosLoc = getAttribLocation("aPosition");
    uTexLoc = getUniformLocation("uTexture");
}

AlPointProgram::~AlPointProgram() {
    vertexSize = 0;
    vertexCount = 0;
    vertex.clear();
}

void AlPointProgram::draw(HwAbsTexture *tex) {
    if (vertex.empty() || vertexSize <= 0 || vertexCount <= 0) {
        return;
    }
    bind();
    if (uTexLoc >= 0) {
        glActiveTexture(GL_TEXTURE0);
        tex->bind();
        glUniform1i(uTexLoc, 0);
    }
    _updateVBOs();
    glEnableVertexAttribArray(aPosLoc);
    glVertexAttribPointer(aPosLoc, vertexSize, GL_FLOAT, GL_FALSE, 0, this->vertex.data());
    glDrawArrays(GL_POINTS, 0, vertexCount);
    glDisableVertexAttribArray(aPosLoc);
    if (uTexLoc >= 0) {
        tex->unbind();
    }
    unbind();
    glFlush();
}

void AlPointProgram::setVertex(std::vector<float> &vertex, int32_t size, int32_t count) {
    if (this->vertex.size() != vertex.size()) {
        this->vertexSize = size;
        this->vertexCount = count;
        this->vertex.clear();
        this->vertex = vertex;
        reqUpdateVertex = true;
    }
}

void AlPointProgram::_updateVBOs() {
    if (!reqUpdateVertex) return;
    reqUpdateVertex = false;
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexSize * vertexCount * 4, this->vertex.data());
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
}

uint32_t AlPointProgram::_createVBOs() {
    return 0;
}
