/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "../include/HwYV122RGBAFilter.h"
#include <GLES2/gl2.h>

static const string VERTEX = HW_SHADER(
        attribute vec4 aPosition;
        attribute vec2 aTextureCoord;
        varying vec2 vTextureCoord;
        void main(void) {
            gl_Position = aPosition;
            vTextureCoord = aTextureCoord;
        }
);

static const string FRAGMENT = HW_SHADER(
        varying vec2 vTextureCoord;
        uniform sampler2D uTexture;//Y
        uniform sampler2D uTexture1;//U
        uniform sampler2D uTexture2;//V
        void main(void) {
            vec3 yuv;
            vec3 rgb;
            yuv.x = texture2D(uTexture, vTextureCoord).r;
            yuv.y = texture2D(uTexture1, vTextureCoord).r - 0.5;
            yuv.z = texture2D(uTexture2, vTextureCoord).r - 0.5;
            rgb = mat3(1, 1, 1,
                       0, -0.39465, 2.03211,
                       1.13983, -0.58060, 0) * yuv;
            gl_FragColor = vec4(rgb, 1);
        }
);

HwYV122RGBAFilter::HwYV122RGBAFilter() : HwAbsFilter() {

}

HwYV122RGBAFilter::~HwYV122RGBAFilter() {

}

HwProgram *HwYV122RGBAFilter::createProgram() {
    string vertex = string(VERTEX);
    string fragment = string(FRAGMENT);
    HwProgram *p = HwProgram::create(&vertex, &fragment);
    this->uLocation = p->getUniformLocation("uTexture1");
    this->vLocation = p->getUniformLocation("uTexture2");
    return p;
}

void HwYV122RGBAFilter::drawFirst(HwProgram *program, HwAbsTexture *src, HwAbsTexture *dest) {
    HwAbsFilter::drawFirst(program, src, dest);
    program->bind();
    glActiveTexture(GL_TEXTURE1);
    this->uTexture->bind();
    glUniform1i(this->uLocation, 1);
    glActiveTexture(GL_TEXTURE2);
    this->vTexture->bind();
    glUniform1i(this->vLocation, 2);
    program->unbind();
}

void HwYV122RGBAFilter::draw(HwAbsTexture *y, HwAbsTexture *u, HwAbsTexture *v, HwAbsTexture *dest) {
    this->uTexture = u;
    this->vTexture = v;
    HwAbsFilter::draw(y, dest);
}