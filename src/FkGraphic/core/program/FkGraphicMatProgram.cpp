/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicMatProgram.h"
#include "FkTexCompo.h"
#include "FkVboCompo.h"
#include "FkMatCompo.h"
#include "FkString.h"
#include "FkTexArrayCompo.h"

FK_IMPL_CLASS_TYPE(FkGraphicMatProgram, FkGraphicProgram)

FkGraphicMatProgram::FkGraphicMatProgram(const FkProgramDescription &desc)
        : FkGraphicProgram(desc) {

}

FkGraphicMatProgram::~FkGraphicMatProgram() {

}

FkProgramDescription::kType FkGraphicMatProgram::type() {
    return FkProgramDescription::kType::MATRIX;
}

FkResult FkGraphicMatProgram::create() {
    auto ret = FkGraphicProgram::create();
    if (FK_OK == ret) {
        aPosLoc = getAttribLocation("aPosition");
        FkAssert(aPosLoc >= 0, FK_FAIL);
        for (int i = 0; i < desc.maxCountOfFragmentTexture; ++i) {
            FkString key("uTexture[");
            key.append(i);
            key.append(']');
            auto loc = getUniformLocation(key.str());
            FkAssert(loc >= 0, FK_FAIL);
            uTexLocArray.emplace_back(loc);
        }
        uMVPMatLoc = getUniformLocation("mvp");
        FkAssert(uMVPMatLoc >= 0, FK_FAIL);
        uColsLoc = getUniformLocation("colsX");
        uRowsLoc = getUniformLocation("rowsY");
        uOffsetWidthLoc = getUniformLocation("offsetWidth");
        uOffsetHeightLoc = getUniformLocation("offsetHeight");
        aCoordinateLoc = getAttribLocation("aTextureCoord");
        FkAssert(aCoordinateLoc >= 0, FK_FAIL);
    }
    return ret;
}

void FkGraphicMatProgram::clear() {
    for (auto itr = values.rbegin(); itr != values.rend(); ++itr) {
        auto it = *itr;
        if (FK_INSTANCE_OF(it, FkVboCompo)) {
            glDisableVertexAttribArray(aPosLoc);
        } else if (FK_INSTANCE_OF(it, FkVboCompo)) {
            glDisableVertexAttribArray(aCoordinateLoc);
        }
    }
    FkGraphicProgram::clear();
}

FkResult FkGraphicMatProgram::addValue(std::shared_ptr<FkComponent> value) {
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
            setUniform1i(uTexLocArray[pValue->index], pValue->index);
        }
    } else if (FK_INSTANCE_OF(value, FkTexArrayCompo)) {
        auto texArrCompo = Fk_POINTER_CAST(FkTexArrayCompo, value);
        if (uColsLoc >= 0) {
            setUniform1i(uColsLoc, texArrCompo->blocks.x);
        }
        if (uRowsLoc >= 0) {
            setUniform1i(uRowsLoc, texArrCompo->blocks.y);
        }
        if (uOffsetWidthLoc >= 0) {
            auto offset = texArrCompo->blockSize.getWidth() * 1.0f / texArrCompo->size.getWidth();
            setUniform1f(uOffsetWidthLoc, offset);
        }
        if (uOffsetHeightLoc >= 0) {
            auto offset = texArrCompo->blockSize.getHeight() * 1.0f / texArrCompo->size.getHeight();
            setUniform1f(uOffsetHeightLoc, offset);
        }
        for (int i = 0; i < texArrCompo->countOfTexture(); ++i) {
            auto compo = std::make_shared<FkTexCompo>((*texArrCompo)[i]);
            compo->index = i;
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
    } else if (FK_INSTANCE_OF(value, FkMatCompo)) {
        auto pValue = Fk_POINTER_CAST(FkMatCompo, value);
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
        void main() {
            gl_Position = mvp * aPosition;
            vTextureCoord = aTextureCoord;
        })");
    return shader;
}

std::string FkGraphicMatProgram::getFragment() {
    std::string shader(R"(
        precision highp float;
        varying highp vec2 vTextureCoord;
        uniform sampler2D uTexture[%d];
        uniform int colsX;
        uniform int rowsY;
        uniform highp float offsetWidth;
        uniform highp float offsetHeight;
        void main() {
            int posX = int(vTextureCoord.x / offsetWidth);
            int posY = int(vTextureCoord.y / offsetHeight);
            vec2 coord = vec2(vTextureCoord.x - offsetWidth * float(posX),
                              vTextureCoord.y - offsetHeight * float(posY));
            if (posX + 1 == colsX) {
                coord = vec2(coord.x / (1.0 - float(posX) * offsetWidth), coord.y);
            } else {
                coord = vec2(coord.x / offsetWidth, coord.y);
            }
            if (posY + 1 == rowsY) {
                coord = vec2(coord.x, coord.y / (1.0 - float(posY) * offsetHeight));
            } else {
                coord = vec2(coord.x, coord.y / offsetHeight);
            }
            int index = posY * colsX + posX;
            vec4 color = texture2D(uTexture[index], coord);
            gl_FragColor = color;
        })");
    size_t size = shader.size() + 10;
    char buf[size];
    memset(buf, 0, size);
    sprintf(buf, shader.c_str(), desc.maxCountOfFragmentTexture);
    shader = std::string(buf);
    return shader;
}
