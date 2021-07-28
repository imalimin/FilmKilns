/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicPointProgram.h"

FkGraphicPointProgram::FkGraphicPointProgram(const FkProgramDescription &desc) : FkGraphicProgram(
        desc) {
    FK_MARK_SUPER
}

FkGraphicPointProgram::~FkGraphicPointProgram() {

}

FkResult FkGraphicPointProgram::create() {
    return FkGraphicProgram::create();
}

void FkGraphicPointProgram::clear() {
    FkGraphicProgram::clear();
}

FkResult FkGraphicPointProgram::addValue(std::shared_ptr<FkProgramValue> value) {
    return FkGraphicProgram::addValue(value);
}

std::string FkGraphicPointProgram::getVertex() {
    std::string shader(R"(
        attribute vec4 aPosition;
        uniform float size;
        void main() {
            vec4 pos = aPosition;
            pos.y = -pos.y;
            gl_Position = pos;
            gl_PointSize = size;
        })");
    return shader;
}

std::string FkGraphicPointProgram::getFragment() {
    std::string shader(R"(
        precision mediump float;
        uniform sampler2D uTexture;
        uniform vec4 color;
        void main() {
                vec4 c = texture2D(uTexture, vec2(gl_PointCoord.x, 1.0 - gl_PointCoord.y));
                gl_FragColor = color * c;
        })");
    return shader;
}
