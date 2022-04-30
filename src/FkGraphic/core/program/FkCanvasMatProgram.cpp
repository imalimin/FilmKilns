/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: ${author}
* CREATE TIME: 2022-4-26 13:18:24
*/

#include "FkCanvasMatProgram.h"
#include "FkGridSizeCompo.h"

FK_IMPL_CLASS_TYPE(FkCanvasMatProgram, FkGraphicMatProgram)

FkCanvasMatProgram::FkCanvasMatProgram(const FkProgramDescription &desc) : FkGraphicMatProgram(desc) {

}

FkCanvasMatProgram::~FkCanvasMatProgram() {

}

FkProgramDescription::kType FkCanvasMatProgram::type() {
    return FkProgramDescription::kType::MATRIX_WITH_CANVAS_BACKGROUND;
}

FkResult FkCanvasMatProgram::create() {
    auto ret = FkGraphicMatProgram::create();
    if (FK_OK == ret) {
        uGridSizeXLoc = getUniformLocation("gridSizeX");
        FkAssert(uGridSizeXLoc >= 0, FK_FAIL);
        uGridSizeYLoc = getUniformLocation("gridSizeY");
        FkAssert(uGridSizeYLoc >= 0, FK_FAIL);
    }
    return ret;
}

FkResult FkCanvasMatProgram::addValue(std::shared_ptr<FkComponent> value) {
    if (nullptr == value) {
        return FK_FAIL;
    }
    if (FK_INSTANCE_OF(value, FkGridSizeCompo)) {
        auto pValue = Fk_POINTER_CAST(FkGridSizeCompo, value);
        setUniform1f(uGridSizeXLoc, pValue->vec.x);
        setUniform1f(uGridSizeYLoc, pValue->vec.y);
    }
    return FkGraphicMatProgram::addValue(value);
}

std::string FkCanvasMatProgram::getFragment() {
    std::string shader(R"(
        precision mediump float;
        varying mediump vec2 vTextureCoord;
        uniform sampler2D uTexture;
        uniform float gridSizeX;
        uniform float gridSizeY;
        void main(){
            vec4 back = vec4(1.0, 1.0, 1.0, 1.0);
            float ix = vTextureCoord.x / gridSizeX;
            float iy = vTextureCoord.y / gridSizeY;
            if (iy / 2.0 - floor(iy / 2.0) >= 0.0 && iy / 2.0 - floor(iy / 2.0) < 0.5) {
                if (ix / 2.0 - floor(ix / 2.0) >= 0.0 && ix / 2.0 - floor(ix / 2.0) < 0.5) {
                    back = vec4(0.796, 0.796, 0.796, 1.0);
                }
            } else {
                if (ix / 2.0 - floor(ix / 2.0) >= 0.5 && ix / 2.0 - floor(ix / 2.0) < 1.0) {
                    back = vec4(0.796, 0.796, 0.796, 1.0);
                }
            }
            vec4 front = texture2D(uTexture, vTextureCoord);
            gl_FragColor = mix(front, back, 1.0 - front.a);
        })");
    return shader;
}