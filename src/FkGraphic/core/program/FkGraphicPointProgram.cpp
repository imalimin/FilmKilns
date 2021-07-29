/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicPointProgram.h"
#include "FkGLDefinition.h"
#include "FkPositionComponent.h"

FkGraphicPointProgram::FkGraphicPointProgram(const FkProgramDescription &desc) : FkGraphicProgram(
        desc) {
    FK_MARK_SUPER
}

FkGraphicPointProgram::~FkGraphicPointProgram() {

}

FkResult FkGraphicPointProgram::create() {
    auto ret = FkGraphicProgram::create();
    if (FK_OK == ret) {
        aPosLoc = getAttribLocation("aPosition");
        FkAssert(aPosLoc >= 0, FK_FAIL);
        uSizeLoc = getAttribLocation("size");
        FkAssert(uSizeLoc >= 0, FK_FAIL);
    }
    return ret;
}

void FkGraphicPointProgram::clear() {
    for (auto itr = values.rbegin(); itr != values.rend(); ++itr) {
        auto it = *itr;
        if (FK_INSTANCE_OF(it, FkPositionComponent)) {
            glDisableVertexAttribArray(aPosLoc);
        }
    }
    FkGraphicProgram::clear();
}

FkResult FkGraphicPointProgram::addValue(std::shared_ptr<FkGraphicComponent> value) {
    if (nullptr == value) {
        return FK_FAIL;
    }
    if (FK_INSTANCE_OF(value, FkPositionComponent)) {
        auto pValue = Fk_POINTER_CAST(FkPositionComponent, value);
        FK_GL_CHECK(glEnableVertexAttribArray(aPosLoc));
        FK_GL_CHECK(glVertexAttribPointer(aPosLoc,
                                          pValue->countPerVertex, GL_FLOAT, GL_FALSE, 0,
                                          reinterpret_cast<const void *>(pValue->offset)));
    }
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
        void main() {
            vec4 color = vec4(1.0 - gl_PointCoord.x, 1.0 - gl_PointCoord.y, 0.0, 1.0);
            gl_FragColor = color;
        })");
    return shader;
}
