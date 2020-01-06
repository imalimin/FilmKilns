/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlMosaicFilter.h"

AlMosaicFilter::AlMosaicFilter() : HwAbsFilter() {

}

AlMosaicFilter::~AlMosaicFilter() {

}

HwProgram *AlMosaicFilter::createProgram() {
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
                    "uniform vec2 radius;\n"
                    "void main() {\n"
                    "    vec2 eachXY = vec2(floor(vTextureCoord.x / radius.x), floor(vTextureCoord.y / radius.y));\n"
                    "    vec2 totalXY = vec2(floor(1.0 / radius.x), floor(1.0 / radius.y));\n"
                    "    vec2 point = vec2(eachXY.x / totalXY.x, eachXY.y / totalXY.y);\n"
                    "    vec4 color = texture2D(uTexture, point);\n"
                    "    gl_FragColor = color;\n"
                    "}");
    HwProgram *program = HwProgram::create(&vertex, &fragment);
    if (program) {
        uRadius = program->getUniformLocation("radius");
    }
    return program;
}

void AlMosaicFilter::drawFirst(HwProgram *program, HwAbsTexture *src, HwAbsTexture *dest) {
    HwAbsFilter::drawFirst(program, src, dest);
    program->bind();
    float size = dest->getWidth() * 0.05f;
    radius.x = size / (float) dest->getWidth();
    radius.y = size / (float) dest->getHeight();
    program->setUniform2fv(uRadius, radius);
}

void AlMosaicFilter::draw(HwAbsTexture *src, HwAbsTexture *dest) {
    HwAbsFilter::draw(src, dest);
}
