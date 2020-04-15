/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlPaintRoundFilter.h"
#include "HwProgram.h"

AlPaintRoundFilter::AlPaintRoundFilter() : HwAbsFilter() {

}

AlPaintRoundFilter::~AlPaintRoundFilter() {

}

AlAbsGLProgram *AlPaintRoundFilter::createProgram() {
    string vertex("attribute vec4 aPosition;\n"
                  "attribute vec2 aTextureCoord;\n"
                  "varying vec2 vTextureCoord;\n"
                  "void main() {\n"
                  "    gl_Position = aPosition;\n"
                  "    vTextureCoord = aTextureCoord;\n"
                  "}");
    string fragment("precision mediump float;\n"
                    "varying mediump vec2 vTextureCoord;\n"
                    "uniform float radius;\n"
                    "void main() {\n"
                    "    vec2 center = vec2(0.5, 0.5);\n"
                    "    if(radius <= length(center - vTextureCoord.xy)) {\n"
                    "        gl_FragColor = vec4(1.0, 1.0, 1.0, 0.0);\n"
                    "    } else {\n"
                    "        gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
                    "    }\n"
                    "}");
    AlAbsGLProgram *program = HwProgram::create(&vertex, &fragment);
    uRadius = program->getUniformLocation("radius");
    return program;
}

void AlPaintRoundFilter::draw(HwAbsTexture *src, HwAbsTexture *dest) {
    HwAbsFilter::draw(src, dest);
}

void AlPaintRoundFilter::drawFirst(AlAbsGLProgram *program, HwAbsTexture *src, HwAbsTexture *dest) {
    HwAbsFilter::drawFirst(program, src, dest);
    program->bind();
    program->setUniform1f(uRadius, 0.5f);
}
