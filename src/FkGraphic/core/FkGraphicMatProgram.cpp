/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicMatProgram.h"
#include "FkTexValue.h"
#include "FkCoordinateValue.h"
#include "FkPositionValue.h"
#include "FkMatrixValue.h"

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
        uMVPMatLoc = getUniformLocation("mvp");
        aTextureCoordinateLocation = getAttribLocation("aTextureCoord");
    }
    return ret;
}

void FkGraphicMatProgram::clear() {
    for (auto itr = values.rbegin(); itr != values.rend(); ++itr) {
        auto it = *itr;
        if (FK_INSTANCE_OF(it, FkPositionValue)) {
            glDisableVertexAttribArray(aPositionLocation);
        } else if (FK_INSTANCE_OF(it, FkCoordinateValue)) {
            glDisableVertexAttribArray(aTextureCoordinateLocation);
        }
    }
    FkGraphicProgram::clear();
}

FkResult FkGraphicMatProgram::addValue(std::shared_ptr<FkProgramValue> value) {
    if (nullptr == value) {
        return FK_FAIL;
    }
    if (FK_INSTANCE_OF(value, FkTexValue)) {
        auto pValue = Fk_POINTER_CAST(FkTexValue, value);
        if (GL_NONE == pValue->tex->tex) {
            glBindTexture(pValue->tex->desc.target, GL_NONE);
        } else {
            glActiveTexture(GL_TEXTURE0 + pValue->index);
            glBindTexture(pValue->tex->desc.target, pValue->tex->tex);
            setUniform1i(uTextureLocation, pValue->index);
        }
    } else if (FK_INSTANCE_OF(value, FkPositionValue)) {
        auto pValue = Fk_POINTER_CAST(FkPositionValue, value);
        glEnableVertexAttribArray(aPositionLocation);
        //xy
        glVertexAttribPointer(aPositionLocation, pValue->countPerVertex, GL_FLOAT, GL_FALSE, 0,
                              reinterpret_cast<const void *>(pValue->offset));
    } else if (FK_INSTANCE_OF(value, FkCoordinateValue)) {
        auto pValue = Fk_POINTER_CAST(FkCoordinateValue, value);
        glEnableVertexAttribArray(aTextureCoordinateLocation);
        //st
        glVertexAttribPointer(aTextureCoordinateLocation, pValue->countPerVertex, GL_FLOAT, GL_FALSE, 0,
                              reinterpret_cast<const void *>(pValue->offset));
    } else if (FK_INSTANCE_OF(value, FkMatrixValue)) {
        auto pValue = Fk_POINTER_CAST(FkMatrixValue, value);
        glUniformMatrix4fv(uMVPMatLoc, 1, GL_FALSE,
                           reinterpret_cast<const GLfloat *>(pValue->mat->get()));
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
