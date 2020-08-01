/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlNV12ToRGBAFilter.h"
#include "HwProgram.h"

AlNV12ToRGBAFilter::AlNV12ToRGBAFilter() : HwAbsFilter() {

}

AlNV12ToRGBAFilter::~AlNV12ToRGBAFilter() {

}

AlAbsGLProgram *AlNV12ToRGBAFilter::createProgram() {
    std::string vertex(R"(
        attribute vec4 aPosition;
        attribute vec2 aTextureCoord;
        varying vec2 vTextureCoord;
        void main(void) {
            gl_Position = aPosition;
            vTextureCoord = aTextureCoord;
        })");
    std::string fragment(R"(
        varying vec2 vTextureCoord;
        uniform sampler2D uTexture;//Y
        uniform sampler2D uTexture1;//UV
        uniform float uStrideOffset;
        void main(void) {
            vec3 yuv;
            vec3 rgb;
            yuv.x = texture2D(uTexture, vTextureCoord).r;
            yuv.y = texture2D(uTexture1, vTextureCoord).r - 0.5;
            yuv.z = texture2D(uTexture1, vTextureCoord).a - 0.5;
            rgb = mat3(1, 1, 1,
                       0, -0.39465, 2.03211,
                       1.13983, -0.58060, 0) * yuv;
            gl_FragColor = vec4(rgb, 1);
        })");
    HwProgram *p = HwProgram::create(&vertex, &fragment);
    this->uvLocation = p->getUniformLocation("uTexture1");
    this->uStrideOffset = p->getUniformLocation("uStrideOffset");
    return p;
}

void AlNV12ToRGBAFilter::drawFirst(AlAbsGLProgram *program, HwAbsTexture *src, HwAbsTexture *dest) {
    HwAbsFilter::drawFirst(program, src, dest);
    program->bind();
    program->setUniform1f(uStrideOffset, 1.0f / src->getWidth());
    glActiveTexture(GL_TEXTURE1);
    this->uvTexture->bind();
    program->setUniform1i(this->uvLocation, 1);
    program->unbind();
}

void AlNV12ToRGBAFilter::draw(HwAbsTexture *y, HwAbsTexture *uv, HwAbsTexture *dest) {
    this->uvTexture = uv;
    HwAbsFilter::draw(y, dest);
}