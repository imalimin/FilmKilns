/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2022-05-08 16:14:18
*/

#include "FkGraphicPathProgram.h"
#include "FkGLDefinition.h"
#include "FkPathCompo.h"
#include "FkSizeCompo.h"
#include "FkColorCompo.h"
#include "FkViewportMatCompo.h"

FK_IMPL_CLASS_TYPE(FkGraphicPathProgram, FkGraphicProgram)

FkGraphicPathProgram::FkGraphicPathProgram(const FkProgramDescription &desc)
        : FkGraphicProgram(desc), size(1, 1) {

}

FkGraphicPathProgram::~FkGraphicPathProgram() {

}
FkProgramDescription::kType FkGraphicPathProgram::type() {
    return FkProgramDescription::kType::PATH;
}

FkResult FkGraphicPathProgram::create() {
    auto ret = FkGraphicProgram::create();
    if (FK_OK == ret) {
        aPosLoc = getAttribLocation("aPosition");
        FkAssert(aPosLoc >= 0, FK_FAIL);
        uPaintColorLoc = getUniformLocation("paintColor");
        FkAssert(uPaintColorLoc >= 0, FK_FAIL);
        uViewportMatLoc = getUniformLocation("viewportMat");
        FkAssert(uViewportMatLoc >= 0, FK_FAIL);
    }
    return ret;
}

void FkGraphicPathProgram::clear() {
    for (auto itr = values.rbegin(); itr != values.rend(); ++itr) {
        auto compo = *itr;
        if (FK_INSTANCE_OF(compo, FkPathCompo)) {
            glDisableVertexAttribArray(aPosLoc);
        }
    }
    FkGraphicProgram::clear();
}

FkResult FkGraphicPathProgram::addValue(std::shared_ptr<FkComponent> value) {
    if (nullptr == value) {
        return FK_FAIL;
    }
    if (FK_INSTANCE_OF(value, FkSizeCompo)) {
        auto pValue = Fk_POINTER_CAST(FkSizeCompo, value);
        size = pValue->size;
    } else if (FK_INSTANCE_OF(value, FkColorCompo)) {
        auto pValue = Fk_POINTER_CAST(FkColorCompo, value);
        std::vector<float> colorVec = {pValue->color.fRed(), pValue->color.fGreen(),
                                       pValue->color.fBlue(), pValue->color.fAlpha()};
        setUniform4fv(uPaintColorLoc, 1, colorVec.data());
    } else if (FK_INSTANCE_OF(value, FkPathCompo)) {
        auto pValue = Fk_POINTER_CAST(FkPathCompo, value);
        std::vector<FkDoubleVec2> points;
        auto count = pValue->readPoints(points);
        if (count * 2 > vertexes.size()) {
            vertexes.resize(count * 2);
        }
        for (int i = 0; i < count; ++i) {
            vertexes[i * 2] = (points[i].x / (size.getWidth() / 2.0));
            vertexes[i * 2 + 1] = (points[i].y / (size.getHeight() / 2.0));
        }

        FK_GL_CHECK(glEnableVertexAttribArray(aPosLoc));
        //xy
        FK_GL_CHECK(glVertexAttribPointer(aPosLoc, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, vertexes.data()));
    } else if (FK_INSTANCE_OF(value, FkViewportMatCompo) && uViewportMatLoc >= 0) {
        auto pValue = Fk_POINTER_CAST(FkViewportMatCompo, value);
        glUniformMatrix4fv(uViewportMatLoc, 1, GL_FALSE,
                           reinterpret_cast<const GLfloat *>(pValue->value->get()));
    }
    return FkGraphicProgram::addValue(value);
}

std::string FkGraphicPathProgram::getVertex() {
    std::string shader(R"(
        precision highp float;
        attribute vec4 aPosition;
        uniform mat4 viewportMat;
        void main() {
            gl_Position = viewportMat * aPosition;
        })");
    return shader;
}

std::string FkGraphicPathProgram::getFragment() {
    std::string shader(R"(
        precision highp float;
        uniform vec4 paintColor;
        void main() {
            gl_FragColor = paintColor;
        })");
    return shader;
}