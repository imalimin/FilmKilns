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
    std::vector<AlVec2 *> positions;
    positions.emplace_back(new AlVec2(0.0f, 0.0f));
    updatePosition(positions);
    size_t size = positions.size();
    for (int i = 0; i < size; ++i) {
        AlVec2 *vec = positions[i];
        delete vec;
    }
    positions.clear();
}

AlPointProgram::~AlPointProgram() {
    posCount = 0;
    if (positions) {
        delete[] positions;
        positions = nullptr;
    }
}

void AlPointProgram::draw(HwAbsTexture *tex) {
    if (nullptr == positions || posCount <= 0){
        return;
    }
    bind();
    if (uTexLoc >= 0) {
        glActiveTexture(GL_TEXTURE0);
        tex->bind();
        glUniform1i(uTexLoc, 0);
    }
    glEnableVertexAttribArray(aPosLoc);
    glVertexAttribPointer(aPosLoc, 2, GL_FLOAT, GL_FALSE, 0, positions);
    glDrawArrays(GL_POINTS, 0, posCount);
    glDisableVertexAttribArray(aPosLoc);
    if (uTexLoc >= 0) {
        tex->unbind();
    }
    unbind();
    glFlush();
}

void AlPointProgram::updatePosition(std::vector<AlVec2 *> &position) {
    if (posCount != position.size()) {
        if (positions) {
            delete[] positions;
        }
        posCount = position.size();
        positions = new float[posCount];
    }
    for (int i = 0; i < posCount; ++i) {
        AlVec2 *vec = position[i];
        positions[i * 2] = vec->x;
        positions[i * 2 + 1] = vec->y;
    }
}
