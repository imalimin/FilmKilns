/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlCanvasDrawer.h"
#include "HwProgram.h"
#include "Logcat.h"
#include "AlVec4.h"

#define TAG "AlCanvasDrawer"

AlCanvasDrawer::AlCanvasDrawer() : HwAbsFilter() {
    _resetUV();
}

AlCanvasDrawer::~AlCanvasDrawer() {

}

AlAbsGLProgram *AlCanvasDrawer::createProgram() {
    string vertex("attribute vec4 aPosition;\n"
                  "attribute vec4 aTextureCoord;\n"
                  "varying vec2 vTextureCoord;\n"
                  "void main() {\n"
                  "    gl_Position = aPosition;\n"
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

void AlCanvasDrawer::drawStart(AlAbsGLProgram *program, HwAbsTexture *src, HwAbsTexture *dest) {
    HwAbsFilter::drawStart(program, src, dest);
    dynamic_cast<HwProgram *>(program)->updateLocation(uv, vertex);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);
}

void AlCanvasDrawer::drawEnd(AlAbsGLProgram *program, HwAbsTexture *src, HwAbsTexture *dest) {
    HwAbsFilter::drawEnd(program, src, dest);
    glDisable(GL_BLEND);
}

void AlCanvasDrawer::setAlpha(float alpha) {
    this->alpha = alpha;
}

void AlCanvasDrawer::setVertexQuad(AlQuad &quad) {
    /// 坐标是正确的，但是不知道为什么用shader绘制上去是上下镜像的样子
    quad.mirrorVertical();
    this->vertex[0] = quad.leftTop().x;
    this->vertex[1] = quad.leftTop().y;
    this->vertex[2] = quad.rightTop().x;
    this->vertex[3] = quad.rightTop().y;
    this->vertex[4] = quad.leftBottom().x;
    this->vertex[5] = quad.leftBottom().y;
    this->vertex[6] = quad.rightBottom().x;
    this->vertex[7] = quad.rightBottom().y;
}

void AlCanvasDrawer::setPositionQuad(AlQuad &quad) {
    if (!quad.isZero()) {
        this->uv[0] = quad.leftTop().x;
        this->uv[1] = quad.leftTop().y;
        this->uv[2] = quad.rightTop().x;
        this->uv[3] = quad.rightTop().y;
        this->uv[4] = quad.leftBottom().x;
        this->uv[5] = quad.leftBottom().y;
        this->uv[6] = quad.rightBottom().x;
        this->uv[7] = quad.rightBottom().y;
    } else {
        _resetUV();
    }
}

void AlCanvasDrawer::_resetUV() {
    AlQuad quad;
    quad.setLeftTop(0.f, 1.f);
    quad.setRightTop(1.f, 1.f);
    quad.setRightBottom(1.f, 0.f);
    quad.setLeftBottom(0.f, 0.f);
    this->uv[0] = quad.leftTop().x;
    this->uv[1] = quad.leftTop().y;
    this->uv[2] = quad.rightTop().x;
    this->uv[3] = quad.rightTop().y;
    this->uv[4] = quad.leftBottom().x;
    this->uv[5] = quad.leftBottom().y;
    this->uv[6] = quad.rightBottom().x;
    this->uv[7] = quad.rightBottom().y;
}
