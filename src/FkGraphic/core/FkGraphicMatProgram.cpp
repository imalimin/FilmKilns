/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicMatProgram.h"

FkGraphicMatProgram::FkGraphicMatProgram(const FkProgramDescription &desc)
        : FkGraphicProgram(desc) {
    FK_MARK_SUPER
    this->desc.type = FkProgramDescription::kType::MATRIX;
}

FkGraphicMatProgram::~FkGraphicMatProgram() {

}

FkResult FkGraphicMatProgram::create() {
    auto ret = FkGraphicProgram::create();
    if (FK_OK == ret) {
        aPositionLocation = getAttribLocation("aPosition");
        uTextureLocation = getUniformLocation("uTexture");
        uMatrixLocation = getUniformLocation("uTextureMatrix");
        aTextureCoordinateLocation = getAttribLocation("aTextureCoord");
    }
    return ret;
}

FkResult FkGraphicMatProgram::bindTexture(int32_t target, int32_t index, FkID tex) {
    if (GL_NONE == tex) {
        glBindTexture(target, GL_NONE);
    } else {
        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(target, tex);
        setUniform1i(uTextureLocation, index);
    }
    return FK_OK;
}

std::string FkGraphicMatProgram::getVertex() {
    std::string shader(R"(
        attribute vec4 aPosition;
        attribute vec2 aTextureCoord;
        varying vec2 vTextureCoord;
        uniform mat4 uTextureMatrix;
        void main(){
            gl_Position= uTextureMatrix * aPosition;
            vTextureCoord = aTextureCoord;
        })");
    return shader;
}

std::string FkGraphicMatProgram::getFragment() {
    std::string shader(R"(
        precision mediump float;
        varying mediump vec2 vTextureCoord;
        uniform sampler2D uTexture;
        void main(){
            vec4 color = vec4(texture2D(uTexture, vTextureCoord).rgb, 1.0);
            gl_FragColor = color;
        })");
    return shader;
}
