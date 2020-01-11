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
    vbo = AlVBO::alloc();
    if (nullptr == vbo) {
        Logcat::e(TAG, "%s(%d) vbo alloc failed", __FUNCTION__, __LINE__);
    }
}

AlPointProgram::~AlPointProgram() {
    delete vbo;
    countPerVertex = 0;
    countVertex = 0;
    vertex.clear();
}

void AlPointProgram::draw(HwAbsTexture *tex) {
    if (vertex.empty() || countPerVertex <= 0 || countVertex <= 0) {
        return;
    }
    bind();
    if (uTexLoc >= 0) {
        glActiveTexture(GL_TEXTURE0);
        tex->bind();
        glUniform1i(uTexLoc, 0);
    }
    vbo->bind();
    _updateVBOs();
    glEnableVertexAttribArray(aPosLoc);
    glVertexAttribPointer(aPosLoc, countPerVertex, GL_FLOAT, GL_FALSE, 0, 0);
    vbo->unbind();
    glDrawArrays(GL_POINTS, 0, countVertex);
    glDisableVertexAttribArray(aPosLoc);
    if (uTexLoc >= 0) {
        tex->unbind();
    }
    unbind();
    glFlush();
}

void AlPointProgram::setVertex(std::vector<float> &vertex, int32_t countPerVertex,
                               int32_t countVertex) {
    if (this->vertex.size() != vertex.size()) {
        this->countPerVertex = countPerVertex;
        this->countVertex = countVertex;
        this->vertex.clear();
        this->vertex = vertex;
        reqUpdateVertex = true;
    }
}

void AlPointProgram::_updateVBOs() {
    if (!reqUpdateVertex || nullptr == vbo) {
        return;
    }
    reqUpdateVertex = false;
    AlBuffer *buf = AlBuffer::wrap(reinterpret_cast<uint8_t *>(this->vertex.data()),
                                   this->vertex.size() * sizeof(float));
    vbo->update(buf);
    delete buf;
}
