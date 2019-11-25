/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlColorGridFilter.h"

AlColorGridFilter *AlColorGridFilter::create() {
    return new AlColorGridFilter();
}

AlColorGridFilter::AlColorGridFilter() : HwAbsFilter() {

}

AlColorGridFilter::~AlColorGridFilter() {
    srcTex = nullptr;
}

void AlColorGridFilter::release(TextureAllocator *texAllocator) {
    texAllocator->recycle(&this->srcTex);
}

bool AlColorGridFilter::prepare(TextureAllocator *texAllocator) {
    uint8_t *bmp = new uint8_t[GRID_WIDTH * GRID_HEIGHT * 4];
    for (int i = 0; i < GRID_WIDTH; ++i) {
        if (i < GRID_WIDTH / 2) {
            bmp[i * 4 + 0] = 255;
            bmp[i * 4 + 1] = 255;
            bmp[i * 4 + 2] = 255;
            bmp[i * 4 + 3] = 255;
        } else {
            bmp[i * 4 + 0] = 203;
            bmp[i * 4 + 1] = 203;
            bmp[i * 4 + 2] = 203;
            bmp[i * 4 + 3] = 255;
        }
    }
    for (int i = 1; i < GRID_HEIGHT; ++i) {
        if (i < GRID_HEIGHT / 2) {
            size_t size = GRID_WIDTH * 4;
            memcpy(bmp + GRID_WIDTH * 4 * i, bmp, size);
        } else {
            size_t size = GRID_WIDTH * 4 / 2;
            memcpy(bmp + GRID_WIDTH * 4 * i, bmp + size, size);
            memcpy(bmp + GRID_WIDTH * 4 * i + size, bmp, size);
        }
    }
    this->srcTex = texAllocator->alloc(bmp, GRID_WIDTH, GRID_HEIGHT, GL_RGBA);
    delete[] bmp;
    return prepare();
}

bool AlColorGridFilter::prepare() {
    return HwAbsFilter::prepare();
}

HwProgram *AlColorGridFilter::createProgram() {
    string vertex("attribute vec4 aPosition;\n"
                  "attribute vec4 aTextureCoord;\n"
                  "varying vec2 vTextureCoord;\n"
                  "void main() {\n"
                  "    gl_Position = aPosition;\n"
                  "    vTextureCoord = aTextureCoord.xy;\n"
                  "}");
    string fragment("precision mediump float;\n"
                    "varying mediump vec2 vTextureCoord;\n"
                    "uniform sampler2D uTexture;\n"
                    "void main() {\n"
                    "    vec4 color = texture2D(uTexture, vTextureCoord);\n"
                    "    gl_FragColor = color;\n"
                    "}");
    return HwProgram::create(&vertex, &fragment);
}

void AlColorGridFilter::drawFirst(HwProgram *program, HwAbsTexture *src, HwAbsTexture *dest) {
    HwAbsFilter::drawFirst(program, src, dest);
    float left = 0.0f;
    float right = dest->getWidth() / (float) GRID_WIDTH;
    float top = dest->getHeight() / (float) GRID_HEIGHT;
    float bottom = 0.0f;
    float *texCoordinate = new float[8]{
            left, bottom,//LEFT,BOTTOM
            right, bottom,//RIGHT,BOTTOM
            left, top,//LEFT,TOP
            right, top//RIGHT,TOP
    };
    program->updateLocation(texCoordinate, nullptr);
}

void AlColorGridFilter::draw(HwAbsTexture *dest) {
    draw(this->srcTex, dest);
}

void AlColorGridFilter::draw(HwAbsTexture *src, HwAbsTexture *dest) {
    HwAbsFilter::draw(src, dest);
}
