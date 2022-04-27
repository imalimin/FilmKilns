/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicPointProgram.h"
#include "FkGLDefinition.h"
#include "FkVboCompo.h"
#include "FkColorCompo.h"
#include "FkSizeCompo.h"
//#include "FkShapeComponent.h"

FkGraphicPointProgram::FkGraphicPointProgram(const FkProgramDescription &desc) : FkGraphicProgram(
        desc) {
    FK_MARK_SUPER
}

FkGraphicPointProgram::~FkGraphicPointProgram() {

}

FkProgramDescription::kType FkGraphicPointProgram::type() {
    return FkProgramDescription::kType::POINT;
}

FkResult FkGraphicPointProgram::create() {
    auto ret = FkGraphicProgram::create();
    if (FK_OK == ret) {
        aPosLoc = getAttribLocation("aPosition");
        FkAssert(aPosLoc >= 0, FK_FAIL);
        uSizeLoc = getUniformLocation("size");
        FkAssert(uSizeLoc >= 0, FK_FAIL);
        uColorLoc = getUniformLocation("color");
        FkAssert(uColorLoc >= 0, FK_FAIL);
        uModeLoc = getUniformLocation("mode");
        FkAssert(uModeLoc >= 0, FK_FAIL);
    }
    return ret;
}

void FkGraphicPointProgram::clear() {
    for (auto itr = values.rbegin(); itr != values.rend(); ++itr) {
        auto it = *itr;
        if (FK_INSTANCE_OF(it, FkVboCompo)) {
            glDisableVertexAttribArray(aPosLoc);
        }
    }
    FkGraphicProgram::clear();
}

FkResult FkGraphicPointProgram::addValue(std::shared_ptr<FkComponent> value) {
    if (nullptr == value) {
        return FK_FAIL;
    }
    if (FK_INSTANCE_OF(value, FkVboCompo)) {
        auto pValue = Fk_POINTER_CAST(FkVboCompo, value);
        int offset = 0;
        FkVertexDesc desc;
        FkAssert(FK_OK == pValue->getValueLoc(FkVboCompo::kValueLoc::VERTEX,
                                              offset, desc), FK_FAIL);

        FK_GL_CHECK(glEnableVertexAttribArray(aPosLoc));
        //xy
        FK_GL_CHECK(glVertexAttribPointer(aPosLoc,
                                          desc.countPerVertex, GL_FLOAT, GL_FALSE, 0,
                                          reinterpret_cast<const void *>(offset)));
    } else if (FK_INSTANCE_OF(value, FkColorCompo)) {
        auto pValue = Fk_POINTER_CAST(FkColorCompo, value);
        FK_GL_CHECK(setUniform4fv(uColorLoc, 1, pValue->color.fArray()));
    } else if (FK_INSTANCE_OF(value, FkSizeCompo)) {
        auto pValue = Fk_POINTER_CAST(FkSizeCompo, value);
        FK_GL_CHECK(setUniform1f(uSizeLoc, pValue->size.getWidth()));
    }
    setUniform1i(uModeLoc, 1);
//    else if (FK_INSTANCE_OF(value, FkShapeComponent)) {
//        auto pValue = Fk_POINTER_CAST(FkShapeComponent, value);
//        FK_GL_CHECK(setUniform1i(uModeLoc, (int) pValue->type));
//    }
    return FkGraphicProgram::addValue(value);
}

std::string FkGraphicPointProgram::getVertex() {
    std::string shader(R"(
        attribute vec4 aPosition;
        uniform float size; // Pixel Size;Rect
        void main() {
            vec4 pos = aPosition;
            pos.y = pos.y;
            gl_Position = pos;
            gl_PointSize = size;
        })");
    return shader;
}

std::string FkGraphicPointProgram::getFragment() {
    std::string shader(R"(
        precision mediump float;
        uniform vec4 color;
        uniform int mode;
        void main() {
            if (1 == mode) {
                float d = abs(distance(vec2(gl_PointCoord.x, gl_PointCoord.y), vec2(0.5, 0.5)));
                if (d > 0.5) {
                    gl_FragColor = vec4(0, 0, 0, 0.0);
                } else {
                    gl_FragColor = color;
                }
            } else {
                gl_FragColor = color;
            }
        })");
    return shader;
}
