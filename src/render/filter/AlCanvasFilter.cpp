/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/AlCanvasFilter.h"

AlCanvasFilter::AlCanvasFilter() : HwAbsFilter() {

}

AlCanvasFilter::~AlCanvasFilter() {

}

HwProgram *AlCanvasFilter::createProgram() {
    string vertex("        attribute vec4 aPosition;\n"
                  "        attribute vec4 aTextureCoord;\n"
                  "        uniform mat4 uTextureMatrix;\n"
                  "        varying vec2 vTextureCoord;\n"
                  "        void main() {\n"
                  "            gl_Position = aPosition;\n"
                  "            vTextureCoord = (uTextureMatrix * aTextureCoord).xy;\n"
                  "        }");
    string fragment("        precision mediump float;\n"
                    "        varying mediump vec2 vTextureCoord;\n"
                    "        uniform sampler2D uTexture;\n"
                    "        void main() {\n"
                    "            vec4 color = texture2D(uTexture, vTextureCoord);\n"
                    "            gl_FragColor = color;\n"
                    "        }");
    return HwProgram::create(&vertex, &fragment);
}

void AlCanvasFilter::drawFirst(HwProgram *program, HwAbsTexture *src, HwAbsTexture *dest) {
    HwAbsFilter::drawFirst(program, src, dest);
    program->updateMatrix(&matrix);
}

void AlCanvasFilter::setScale(float scale) {
    matrix.scale(scale, scale, 1.0f);
}
