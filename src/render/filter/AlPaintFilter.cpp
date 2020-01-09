/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlPaintFilter.h"
#include "AlPointProgram.h"
#include "Logcat.h"

#define TAG "AlPaintFilter"

AlPaintFilter::AlPaintFilter() : HwAbsFilter() {

}

AlPaintFilter::~AlPaintFilter() {
    path = nullptr;
}

void AlPaintFilter::setPath(std::vector<AlVec2> *vec) {
    path = vec;
}

AlAbsGLProgram *AlPaintFilter::createProgram() {
    string vertex("attribute vec4 aPosition;\n"
                  "void main() {\n"
                  "    gl_Position = aPosition;\n"
                  "    gl_PointSize = 100.0;\n"
                  "}");
    string fragment("precision mediump float;\n"
                    "void main() {\n"
                    "    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
                    "}");
    return AlPointProgram::create(&vertex, &fragment);
}

void AlPaintFilter::drawFirst(AlAbsGLProgram *program, HwAbsTexture *src, HwAbsTexture *dest) {
    HwAbsFilter::drawFirst(program, src, dest);
    if (path) {
        program->bind();
        dynamic_cast<AlPointProgram *>(program)->updatePosition(*path);
        Logcat::i(TAG, "%s(%d) size %d", __FUNCTION__, __LINE__, path->size());
    }
}

void AlPaintFilter::draw(HwAbsTexture *src, HwAbsTexture *dest) {
    HwAbsFilter::draw(src, dest);
}
