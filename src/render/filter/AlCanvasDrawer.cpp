/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlCanvasDrawer.h"
#include "Logcat.h"
#include "AlVec4.h"

#define TAG "AlCanvasDrawer"

AlCanvasDrawer::AlCanvasDrawer() : HwAbsFilter() {

}

AlCanvasDrawer::~AlCanvasDrawer() {

}

HwProgram *AlCanvasDrawer::createProgram() {
    string vertex("attribute vec4 aPosition;\n"
                  "attribute vec4 aTextureCoord;\n"
                  "uniform mat4 uTextureMatrix;\n"
                  "varying vec2 vTextureCoord;\n"
                  "void main() {\n"
                  "    gl_Position = uTextureMatrix * aPosition;\n"
                  "    vTextureCoord = aTextureCoord.xy;\n"
                  "}");
    string fragment("precision mediump float;\n"
                    "varying mediump vec2 vTextureCoord;\n"
                    "uniform sampler2D uTexture;\n"
                    "void main() {\n"
                    "    vec4 color = texture2D(uTexture, vTextureCoord);\n"
                    "    gl_FragColor = color;\n"
                    "}");
    return HwProgram::create(&vertex, &fragment);
}

void AlCanvasDrawer::drawFirst(HwProgram *program, HwAbsTexture *src, HwAbsTexture *dest) {
    auto *m = HwMatrix::fromArray(matrix.data());
    program->updateMatrix(m);
    program->updateLocation(nullptr, vertex);
    delete m;
}

void AlCanvasDrawer::setAlpha(float alpha) {
    this->alpha = alpha;
}

void AlCanvasDrawer::setMatrix(AlMatrix &matrix) {
    this->matrix = matrix;
}

void AlCanvasDrawer::setVertexRectF(AlRectF &rectF) {
    this->vertex[0] = rectF.left;
    this->vertex[1] = rectF.bottom;
    this->vertex[2] = rectF.right;
    this->vertex[3] = rectF.bottom;
    this->vertex[4] = rectF.left;
    this->vertex[5] = rectF.top;
    this->vertex[6] = rectF.right;
    this->vertex[7] = rectF.top;
}

void AlCanvasDrawer::setPositionRectF(AlRectF &rectF) {

}
