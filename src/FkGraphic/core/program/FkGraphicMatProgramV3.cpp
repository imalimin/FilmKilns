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
    }
    FkGraphicProgram::clear();
}

FkResult FkGraphicMatProgramV3::addValue(std::shared_ptr<FkComponent> value) {
    if (nullptr == value) {
        return FK_FAIL;
    }
    return FkGraphicProgram::addValue(value);
}

std::string FkGraphicMatProgramV3::getVertex() {
    std::string shader(R"(
        #version 300 es
        layout(location = 0) vec4 aPosition;
        layout(location = 1) vec2 aTextureCoord;
        uniform mat4 mvp;
        out vec2 vTextureCoord;
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
        layout(location = 1) out vec4 fragColor1;
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
            vec4 color = texture(uTexture[index], coord);
            fragColor0 = color;
        })");
    size_t size = shader.size() + 10;
    char buf[size];
    memset(buf, 0, size);
    sprintf(buf, shader.c_str(), desc.maxCountOfFragmentTexture);
    shader = std::string(buf);
    return shader;
}