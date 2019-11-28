/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlCanvasDrawer.h"

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
    HwAbsFilter::drawFirst(program, src, dest);
    int32_t width = dest->getWidth();
    int32_t height = dest->getHeight();
    float aspectRatio = width > height ?
                        (float) width / (float) height :
                        (float) height / (float) width;

    float left = -1.0f;
    float right = -left;
    float bottom = -1.0f;
    float top = -bottom;
    if (width > height) {
        matrix.update(-aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
        left = -src->getWidth() / (float) src->getHeight();
        right = -left;
        bottom = -1.0f;
        top = -bottom;
    } else {
        matrix.update(-1.0f, 1.0f, -aspectRatio, aspectRatio, -1.0f, 1.0f);
        left = -1.0f;
        right = -left;
        bottom = -src->getHeight() / (float) src->getWidth();
        top = -bottom;
    }
    auto *m = HwMatrix::fromArray(matrix.data());
    program->updateMatrix(m);
    delete m;
    float *vertex = new float[8]{
            left, bottom, //LEFT,BOTTOM
            right, bottom, //RIGHT,BOTTOM
            left, top, //LEFT,TOP
            right, top,//RIGHT,TOP
    };
    program->updateLocation(nullptr, vertex);
    delete[] vertex;
}

void AlCanvasDrawer::setScale(float scaleX, float scaleY) {
    matrix.setScale(scaleX, scaleY);
}

void AlCanvasDrawer::setRotation(float rotation) {
    matrix.setRotation(rotation);
}

void AlCanvasDrawer::setTranslate(float x, float y) {
    matrix.setTranslate(x, y);
    matrix.dump();
}

void AlCanvasDrawer::setAlpha(float alpha) {
    this->alpha = alpha;
}
