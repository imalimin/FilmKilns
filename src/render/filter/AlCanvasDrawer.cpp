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
    HwAbsFilter::drawFirst(program, src, dest);
    AlSize sSize(src->getWidth(), src->getHeight());
    AlSize dSize(dest->getWidth(), dest->getHeight());
    _calculateRect(sSize, dSize);
    oMat.update(dRectF.left, dRectF.right, dRectF.bottom, dRectF.top, -1.0f, 1.0f);
    AlMatrix mat = oMat * tMat;
    auto *m = HwMatrix::fromArray(mat.data());
    program->updateMatrix(m);
    delete m;
    float *vertex = new float[8]{
            sRectF.left, sRectF.bottom, //LEFT,BOTTOM
            sRectF.right, sRectF.bottom, //RIGHT,BOTTOM
            sRectF.left, sRectF.top, //LEFT,TOP
            sRectF.right, sRectF.top,//RIGHT,TOP
    };
    AlVec4 leftTop(sRectF.left, sRectF.top);
    leftTop.multiply(mat);
    leftTop.dump();
    program->updateLocation(nullptr, vertex);
    delete[] vertex;
}

void AlCanvasDrawer::_calculateRect(AlSize &src, AlSize &dest) {
    float aspectRatio = dest.width > dest.height ?
                        (float) dest.width / (float) dest.height :
                        (float) dest.height / (float) dest.width;
    if (dest.width > dest.height) {
        //计算正交矩阵
        dRectF.left = -aspectRatio;
        dRectF.right = -dRectF.left;
        dRectF.bottom = -1.0f;
        dRectF.top = -dRectF.bottom;
        //计算顶点
        sRectF.left = -src.width / (float) src.height;
        sRectF.right = -sRectF.left;
        sRectF.bottom = -1.0f;
        sRectF.top = -sRectF.bottom;
    } else {
        dRectF.left = -1.0f;
        dRectF.right = -dRectF.left;
        dRectF.bottom = -aspectRatio;
        dRectF.top = -dRectF.bottom;
        sRectF.left = -1.0f;
        sRectF.right = -sRectF.left;
        sRectF.bottom = -src.height / (float) src.width;
        sRectF.top = -sRectF.bottom;
    }

}

void AlCanvasDrawer::setScale(float scaleX, float scaleY) {
    tMat.setScale(scaleX, scaleY);
}

void AlCanvasDrawer::setRotation(float rotation) {
    tMat.setRotation(rotation);
}

void AlCanvasDrawer::setTranslate(float x, float y) {
    tMat.setTranslate(x * dRectF.getWidth() / 2.0f, y * dRectF.getHeight() / 2.0f);
    tMat.dump();
}

void AlCanvasDrawer::setAlpha(float alpha) {
    this->alpha = alpha;
}
