/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-03-16 22:11:03
*/

#include "FkGraphicMatAndroidProgram.h"
#include "FkGLDefinition.h"
#include "FkTexCompo.h"
#include "FkVboCompo.h"
#include "FkMatCompo.h"
#include "FkString.h"
#include "FkTexArrayCompo.h"
#include "FkViewportMatCompo.h"
#include "FkTransMatCompo.h"

FK_IMPL_CLASS_TYPE(FkGraphicMatAndroidProgram, FkGraphicProgram)

FkGraphicMatAndroidProgram::FkGraphicMatAndroidProgram(const FkProgramDescription &desc)
        : FkGraphicProgram(desc) {

}

FkGraphicMatAndroidProgram::~FkGraphicMatAndroidProgram() {

}

FkProgramDescription::kType FkGraphicMatAndroidProgram::type() {
    return FkProgramDescription::kType::MATRIX_ANDROID_IMAGE;
}

FkResult FkGraphicMatAndroidProgram::create() {
    auto ret = FkGraphicProgram::create();
    if (FK_OK == ret) {
        aPosLoc = getAttribLocation("aPosition");
        FkAssert(aPosLoc >= 0, FK_FAIL);
        uTexLoc = getUniformLocation("uTexture");
        FkAssert(uTexLoc >= 0, FK_FAIL);
        uMVPMatLoc = getUniformLocation("mvp");
        FkAssert(uMVPMatLoc >= 0, FK_FAIL);
        uTransMatrixLoc = getUniformLocation("uTransMatrix");
        FkAssert(uTransMatrixLoc >= 0, FK_FAIL);
        aCoordinateLoc = getAttribLocation("aTextureCoord");
        FkAssert(aCoordinateLoc >= 0, FK_FAIL);
    }
    return ret;
}

void FkGraphicMatAndroidProgram::clear() {
    for (auto itr = values.rbegin(); itr != values.rend(); ++itr) {
        auto it = *itr;
        if (FK_INSTANCE_OF(it, FkVboCompo)) {
            glDisableVertexAttribArray(aPosLoc);
            glDisableVertexAttribArray(aCoordinateLoc);
        }
    }
    FkGraphicProgram::clear();
}

FkResult FkGraphicMatAndroidProgram::addValue(std::shared_ptr<FkComponent> value) {
    if (nullptr == value) {
        return FK_FAIL;
    }
    if (FK_INSTANCE_OF(value, FkTexCompo)) {
        auto pValue = Fk_POINTER_CAST(FkTexCompo, value);
        if (GL_NONE == pValue->tex->tex) {
            glBindTexture(pValue->tex->desc.target, GL_NONE);
        } else {
            glActiveTexture(GL_TEXTURE0 + pValue->index);
            glBindTexture(pValue->tex->desc.target, pValue->tex->tex);
            setUniform1i(uTexLoc, pValue->index);
        }
    } else if (FK_INSTANCE_OF(value, FkTexArrayCompo)) {
        auto texArrCompo = Fk_POINTER_CAST(FkTexArrayCompo, value);
        if(texArrCompo && texArrCompo->countOfTexture() > 0) {
            auto compo = std::make_shared<FkTexCompo>((*texArrCompo)[0]);
            compo->index = 0;
            addValue(compo);
        }
    } else if (FK_INSTANCE_OF(value, FkVboCompo)) {
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

        offset = 0;
        FkAssert(FK_OK == pValue->getValueLoc(FkVboCompo::kValueLoc::COORDINATE,
                                              offset, desc), FK_FAIL);
        FK_GL_CHECK(glEnableVertexAttribArray(aCoordinateLoc));
        //st
        FK_GL_CHECK(glVertexAttribPointer(aCoordinateLoc,
                                          desc.countPerVertex, GL_FLOAT, GL_FALSE, 0,
                                          reinterpret_cast<const void *>(offset)));
    } else if (FK_INSTANCE_OF(value, FkTransMatCompo)) {
        auto pValue = Fk_POINTER_CAST(FkTransMatCompo, value);
        glUniformMatrix4fv(uTransMatrixLoc, 1, GL_FALSE,
                           reinterpret_cast<const GLfloat *>(pValue->value->get()));
    } else if (FK_INSTANCE_OF(value, FkMatCompo)) {
        auto pValue = Fk_POINTER_CAST(FkMatCompo, value);
        glUniformMatrix4fv(uMVPMatLoc, 1, GL_FALSE,
                           reinterpret_cast<const GLfloat *>(pValue->value->get()));
    }
    return FkGraphicProgram::addValue(value);
}

std::string FkGraphicMatAndroidProgram::getVertex() {
    std::string shader(R"(
        attribute vec4 aPosition;
        attribute vec4 aTextureCoord;
        varying vec2 vTextureCoord;
        uniform mat4 uTransMatrix;
        uniform mat4 mvp;
        void main(){
            gl_Position =  mvp * aPosition;
            vTextureCoord = (uTransMatrix * aTextureCoord).xy;
        })");
    return shader;
}

std::string FkGraphicMatAndroidProgram::getFragment() {
    std::string shader(R"(
        #extension GL_OES_EGL_image_external : require
        precision mediump float;
        varying mediump vec2 vTextureCoord;
        uniform samplerExternalOES uTexture;
        void main(){
            vec4 color = vec4(texture2D(uTexture, vTextureCoord).rgb, 1.0);
            gl_FragColor = color;
        })");
    return shader;
}