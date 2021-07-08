/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkCanvasMosaicProgram.h"
#include "FkPositionValue.h"

FkCanvasMosaicProgram::FkCanvasMosaicProgram(const FkProgramDescription &desc) : FkGraphicProgram(desc) {
    FK_MARK_SUPER
}

FkCanvasMosaicProgram::~FkCanvasMosaicProgram() {

}

FkResult FkCanvasMosaicProgram::create() {
    auto ret = FkGraphicProgram::create();
    if (FK_OK == ret) {
        aPositionLocation = getAttribLocation("aPosition");
    }
    return ret;
}

void FkCanvasMosaicProgram::clear() {
    for (auto itr = values.rbegin(); itr != values.rend(); ++itr) {
        auto it = *itr;
        if (FK_INSTANCE_OF(it, FkPositionValue)) {
            glDisableVertexAttribArray(aPositionLocation);
        }
    }
    FkGraphicProgram::clear();
}

FkResult FkCanvasMosaicProgram::addValue(std::shared_ptr<FkProgramValue> value) {
    if (nullptr == value) {
        return FK_FAIL;
    }
    if (FK_INSTANCE_OF(value, FkPositionValue)) {
        auto pValue = Fk_POINTER_CAST(FkPositionValue, value);
        glEnableVertexAttribArray(aPositionLocation);
        //xy
        glVertexAttribPointer(aPositionLocation, pValue->countPerVertex, GL_FLOAT, GL_FALSE, 0,
                              reinterpret_cast<const void *>(pValue->offset));
    }
    return FkGraphicProgram::addValue(value);
}

std::string FkCanvasMosaicProgram::getVertex() {
    std::string shader(R"(
        attribute vec4 aPosition;
        void main(){
            gl_Position = aPosition;
        })");
    return shader;
}

std::string FkCanvasMosaicProgram::getFragment() {
    std::string shader(R"(
        precision mediump float;
        void main(){
            gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
        })");
    return shader;
}