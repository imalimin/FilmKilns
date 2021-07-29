/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicMatProgram.h"
#include "FkGraphicTexComponent.h"
#include "FkCoordinateComponent.h"
#include "FkPositionComponent.h"
#include "FkMatrixComponent.h"

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
        aPosLoc = getAttribLocation("aPosition");
        FkAssert(aPosLoc >= 0, FK_FAIL);
        uTextureLoc = getUniformLocation("uTexture");
        FkAssert(uTextureLoc >= 0, FK_FAIL);
        uMVPMatLoc = getUniformLocation("mvp");
        FkAssert(uMVPMatLoc >= 0, FK_FAIL);
        aCoordinateLoc = getAttribLocation("aTextureCoord");
        FkAssert(aCoordinateLoc >= 0, FK_FAIL);
    }
    return ret;
}

void FkGraphicMatProgram::clear() {
    for (auto itr = values.rbegin(); itr != values.rend(); ++itr) {
        auto it = *itr;
        if (FK_INSTANCE_OF(it, FkPositionComponent)) {
            glDisableVertexAttribArray(aPosLoc);
        } else if (FK_INSTANCE_OF(it, FkCoordinateComponent)) {
            glDisableVertexAttribArray(aCoordinateLoc);
        }
    }
    FkGraphicProgram::clear();
}

FkResult FkGraphicMatProgram::addValue(std::shared_ptr<FkGraphicComponent> value) {
    if (nullptr == value) {
        return FK_FAIL;
    }
    if (FK_INSTANCE_OF(value, FkGraphicTexComponent)) {
        auto pValue = Fk_POINTER_CAST(FkGraphicTexComponent, value);
        if (GL_NONE == pValue->tex->tex) {
            glBindTexture(pValue->tex->desc.target, GL_NONE);
        } else {
            glActiveTexture(GL_TEXTURE0 + pValue->index);
            glBindTexture(pValue->tex->desc.target, pValue->tex->tex);
            setUniform1i(uTextureLoc, pValue->index);
        }
    } else if (FK_INSTANCE_OF(value, FkPositionComponent)) {
        auto pValue = Fk_POINTER_CAST(FkPositionComponent, value);
        glEnableVertexAttribArray(aPosLoc);
        //xy
        glVertexAttribPointer(aPosLoc, pValue->countPerVertex, GL_FLOAT, GL_FALSE, 0,
                              reinterpret_cast<const void *>(pValue->offset));
    } else if (FK_INSTANCE_OF(value, FkCoordinateComponent)) {
        auto pValue = Fk_POINTER_CAST(FkCoordinateComponent, value);
        glEnableVertexAttribArray(aCoordinateLoc);
        //st
        glVertexAttribPointer(aCoordinateLoc, pValue->countPerVertex, GL_FLOAT, GL_FALSE, 0,
                              reinterpret_cast<const void *>(pValue->offset));
    } else if (FK_INSTANCE_OF(value, FkMatrixComponent)) {
        auto pValue = Fk_POINTER_CAST(FkMatrixComponent, value);
        glUniformMatrix4fv(uMVPMatLoc, 1, GL_FALSE,
                           reinterpret_cast<const GLfloat *>(pValue->value->get()));
    }
    return FkGraphicProgram::addValue(value);
}

std::string FkGraphicMatProgram::getVertex() {
    std::string shader(R"(
        attribute vec4 aPosition;
        attribute vec2 aTextureCoord;
        varying vec2 vTextureCoord;
        uniform mat4 mvp;
        void main(){
            gl_Position = mvp * aPosition;
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
