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

void AlPaintFilter::setPath(std::vector<float> *vec) {
    path = vec;
}

void AlPaintFilter::setPaintSize(float size) {
    this->paintSize = size;
}

void AlPaintFilter::setColor(AlColor color) {
    this->color = color;
}

AlAbsGLProgram *AlPaintFilter::createProgram() {
    string vertex("attribute vec4 aPosition;\n"
                  "uniform float size;\n"
                  "void main() {\n"
                  "    gl_Position = aPosition;\n"
                  "    gl_PointSize = size;\n"
                  "}");
    string fragment("precision mediump float;\n"
                    "uniform vec4 color;\n"
                    "void main() {\n"
                    "    gl_FragColor = color;\n"
                    "}");
    AlAbsGLProgram *program = AlPointProgram::create(&vertex, &fragment);
    uSize = program->getUniformLocation("size");
    uColor = program->getUniformLocation("color");
    return program;
}

void AlPaintFilter::drawFirst(AlAbsGLProgram *program, HwAbsTexture *src, HwAbsTexture *dest) {
    HwAbsFilter::drawFirst(program, src, dest);
    program->bind();
    if (path) {
        dynamic_cast<AlPointProgram *>(program)->setVertex(*path, 2, path->size() / 2);
    }
    float color[4];
    color[0] = this->color.rf();
    color[1] = this->color.gf();
    color[2] = this->color.bf();
    color[3] = 1.0f - this->color.af();
    program->setUniform4fv(uColor, 1, color);
    program->setUniform1f(uSize, paintSize * dest->getWidth());
}

void AlPaintFilter::draw(HwAbsTexture *src, HwAbsTexture *dest) {
    HwAbsFilter::draw(src, dest);
}
