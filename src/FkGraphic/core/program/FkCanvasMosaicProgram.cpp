/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkCanvasMosaicProgram.h"
#include "FkVboCompo.h"

FkCanvasMosaicProgram::FkCanvasMosaicProgram(const FkProgramDescription &desc) : FkGraphicProgram(desc) {
    FK_MARK_SUPER
}

FkCanvasMosaicProgram::~FkCanvasMosaicProgram() {

}

FkResult FkCanvasMosaicProgram::create() {
    auto ret = FkGraphicProgram::create();
    if (FK_OK == ret) {
        aPosLoc = getAttribLocation("aPosition");
        FkAssert(aPosLoc >= 0, FK_FAIL);
        aCoordinateLoc = getAttribLocation("aTextureCoord");
        FkAssert(aCoordinateLoc >= 0, FK_FAIL);
    }
    return ret;
}

void FkCanvasMosaicProgram::clear() {
    for (auto itr = values.rbegin(); itr != values.rend(); ++itr) {
        auto it = *itr;
        if (FK_INSTANCE_OF(it, FkVboCompo)) {
            glDisableVertexAttribArray(aPosLoc);
            glDisableVertexAttribArray(aCoordinateLoc);
        }
    }
    FkGraphicProgram::clear();
}

FkResult FkCanvasMosaicProgram::addValue(std::shared_ptr<FkComponent> value) {
    if (nullptr == value) {
        return FK_FAIL;
    }
    if (FK_INSTANCE_OF(value, FkVboCompo)) {
        float pos[]{
                -1.0f, -1.0f,//LEFT,BOTTOM
                1.0f / 2.0f, -1.0f,//RIGHT,BOTTOM
                -1.0f / 2.0f, 1.0f,//LEFT,TOP
                1.0f / 2.0f, 1.0f//RIGHT,TOP
        };
        float coordinate[]{
                0.0f, 0.0f,//LEFT,BOTTOM
                1.0f, 0.0f,//RIGHT,BOTTOM
                0.0f, 1.0f,//LEFT,TOP
                1.0f, 1.0f//RIGHT,TOP
        };
        glEnableVertexAttribArray(aPosLoc);
        //st
        glVertexAttribPointer(aPosLoc, 2, GL_FLOAT, GL_FALSE, 2 * 4 * sizeof(float), pos);
        glEnableVertexAttribArray(aCoordinateLoc);
        //st
        glVertexAttribPointer(aCoordinateLoc, 2, GL_FLOAT, GL_FALSE, 2 * 4 * sizeof(float), coordinate);
    }
    return FkGraphicProgram::addValue(value);
}

std::string FkCanvasMosaicProgram::getVertex() {
    std::string shader(R"(
        attribute vec4 aPosition;
        attribute vec2 aTextureCoord;
        varying vec2 vTextureCoord;
        void main(){
            gl_Position = aPosition;
            vTextureCoord = aTextureCoord;
        })");
    return shader;
}

std::string FkCanvasMosaicProgram::getFragment() {
    std::string shader(R"(
        precision mediump float;
        varying mediump vec2 vTextureCoord;
        void main(){
            vec4 color = vec4(1.0 - vTextureCoord.x, 1.0 - vTextureCoord.y, 0.0, 1.0);
            gl_FragColor = color;
        })");
    return shader;
}