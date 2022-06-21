/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2022-06-19 16:17:35
*/

#include "FkGraphicMatProgramV3.h"
#include "FkTexCompo.h"
#include "FkVboCompo.h"
#include "FkMatCompo.h"
#include "FkString.h"
#include "FkTexArrayCompo.h"

FK_IMPL_CLASS_TYPE(FkGraphicMatProgramV3, FkGraphicMatProgram)

FkGraphicMatProgramV3::FkGraphicMatProgramV3(const FkProgramDescription &desc)
        : FkGraphicMatProgram(desc) {

}

FkGraphicMatProgramV3::~FkGraphicMatProgramV3() {

}

FkProgramDescription::kType FkGraphicMatProgramV3::type() {
    return FkProgramDescription::kType::MATRIX_V3;
}

FkResult FkGraphicMatProgramV3::create() {
    auto ret = FkGraphicProgram::create();
    if (FK_OK == ret) {
        aPosLoc = 0;
        aCoordinateLoc = 1;
        uMVPMatLoc = getUniformLocation("mvp");
        FkAssert(uMVPMatLoc >= 0, FK_FAIL);
        for (int i = 0; i < desc.maxCountOfFragmentTexture; ++i) {
            FkString key("uTexture[");
            key.append(i);
            key.append(']');
            auto loc = getUniformLocation(key.str());
            FkAssert(loc >= 0, FK_FAIL);
            uTexLocArray.emplace_back(loc);
        }
        uColsLoc = getUniformLocation("colsX");
        uRowsLoc = getUniformLocation("rowsY");
        uOffsetWidthLoc = getUniformLocation("offsetWidth");
        uOffsetHeightLoc = getUniformLocation("offsetHeight");
    }
    return ret;
}

void FkGraphicMatProgramV3::clear() {
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

FkResult FkGraphicMatProgramV3::addValue(std::shared_ptr<FkComponent> value) {
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

std::string FkGraphicMatProgramV3::getVertex() {
    std::string shader(R"(
        #version 300 es
        layout(location = 0) vec4 aPosition;
        layout(location = 1) vec2 aTextureCoord;
        uniform mat4 mvp;
        out highp vec2 vTextureCoord;
        void main() {
            gl_Position = mvp * aPosition;
            vTextureCoord = aTextureCoord;
        })");
    return shader;
}

std::string FkGraphicMatProgramV3::getFragment() {
    std::string shader(R"(
        #version 300 es
        precision highp float;
        in highp vec2 vTextureCoord;
        uniform sampler2D uTexture[%d];
        uniform int colsX;
        uniform int rowsY;
        uniform highp float offsetWidth;
        uniform highp float offsetHeight;
        layout(location = 0) out vec4 fragColor0;
        vec4 getSamplerColor(int index, vec2 coord) {
            vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
            if (index == 1) {
                color = texture(uTexture[1], coord);
            } else if (index == 2) {
                color = texture(uTexture[2], coord);
            } else if (index == 3) {
                color = texture(uTexture[3], coord);
            } else if (index == 4) {
                color = texture(uTexture[4], coord);
            } else if (index == 5) {
                color = texture(uTexture[5], coord);
            } else if (index == 6) {
                color = texture(uTexture[6], coord);
            } else if (index == 7) {
                color = texture(uTexture[7], coord);
            } else if (index == 8) {
                color = texture(uTexture[8], coord);
            } else if (index == 9) {
                color = texture(uTexture[9], coord);
            } else if (index == 10) {
                color = texture(uTexture[10], coord);
            } else if (index == 11) {
                color = texture(uTexture[11], coord);
            } else if (index == 12) {
                color = texture(uTexture[12], coord);
            } else if (index == 13) {
                color = texture(uTexture[13], coord);
            } else if (index == 14) {
                color = texture(uTexture[14], coord);
            } else if (index == 15) {
                color = texture(uTexture[15], coord);
            } else {
                color = texture(uTexture[0], coord);
            }
            return color;
        }
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
            vec4 color = getSamplerColor(index, coord);
            fragColor0 = color;
        }
        )");
    size_t size = shader.size() + 10;
    char buf[size];
    memset(buf, 0, size);
    sprintf(buf, shader.c_str(), desc.maxCountOfFragmentTexture);
    shader = std::string(buf);
    return shader;
}