/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "../include/HwNormalFilter.h"

static const string VERTEX = HW_SHADER(
        attribute vec4 aPosition;
        attribute vec2 aTextureCoord;
        varying vec2 vTextureCoord;
        void main() {
            gl_Position= aPosition;
            vTextureCoord = aTextureCoord;
        }
);

static const string FRAGMENT = HW_SHADER(
        precision mediump float;
        varying mediump vec2 vTextureCoord;
        uniform sampler2D uTexture;
        void main() {
            vec4 color = vec4(texture2D(uTexture, vTextureCoord).rgb, 1.0);
            gl_FragColor = color;
        }
);

HwNormalFilter::HwNormalFilter() : HwAbsFilter() {
    name = __func__;
}

HwNormalFilter::~HwNormalFilter() {

}

HwProgram *HwNormalFilter::createProgram() {
    string vertex = string(VERTEX);
    string fragment = string(FRAGMENT);
    return HwProgram::create(&vertex, &fragment);;
}