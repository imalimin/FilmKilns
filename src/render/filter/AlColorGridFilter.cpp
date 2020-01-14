/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlColorGridFilter.h"
#include "HwProgram.h"

AlColorGridFilter *AlColorGridFilter::create() {
    return new AlColorGridFilter();
}

AlColorGridFilter::AlColorGridFilter() : HwAbsFilter() {

}

AlColorGridFilter::~AlColorGridFilter() {
    srcTex = nullptr;
}

bool AlColorGridFilter::prepare(HwAbsTexture *tex) {
    this->srcTex = tex;
    return prepare();
}

bool AlColorGridFilter::prepare() {
    return HwAbsFilter::prepare();
}

void AlColorGridFilter::_update(AlSize &canvasSize) {
    if (this->canvasSize == canvasSize) {
        return;
    }
    this->canvasSize.width = canvasSize.width;
    this->canvasSize.height = canvasSize.height;
    this->size = GRID_SIZE * canvasSize.width / 1080;
    if (0 != this->size % 2) {
        size += 1;
    }
    auto *buf = AlBuffer::alloc(size * size * 4);
    auto *bmp = buf->data();
    for (int i = 0; i < size; ++i) {
        if (i < size / 2) {
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
    for (int i = 1; i < size; ++i) {
        if (i < size / 2) {
            size_t len = size * 4;
            memcpy(bmp + size * 4 * i, bmp, len);
        } else {
            size_t len = size * 4 / 2;
            memcpy(bmp + size * 4 * i, bmp + len, len);
            memcpy(bmp + size * 4 * i + len, bmp, len);
        }
    }
    this->srcTex->update(buf, size, size, GL_RGBA);
    delete buf;
}

AlAbsGLProgram *AlColorGridFilter::createProgram() {
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

void AlColorGridFilter::drawFirst(AlAbsGLProgram *program, HwAbsTexture *src, HwAbsTexture *dest) {
    HwAbsFilter::drawFirst(program, src, dest);
    float left = 0.0f;
    float right = dest->getWidth() / (float) size;
    float top = dest->getHeight() / (float) size;
    float bottom = 0.0f;
    float *texCoordinate = new float[8]{
            left, top,//LEFT,TOP
            right, top,//RIGHT,TOP
            left, bottom,//LEFT,BOTTOM
            right, bottom,//RIGHT,BOTTOM
    };
    dynamic_cast<HwProgram *>(program)->updateLocation(texCoordinate, nullptr);
}

void AlColorGridFilter::draw(HwAbsTexture *dest) {
    AlSize size(dest->getWidth(), dest->getHeight());
    _update(size);
    draw(this->srcTex, dest);
}

void AlColorGridFilter::draw(HwAbsTexture *src, HwAbsTexture *dest) {
    HwAbsFilter::draw(src, dest);
}
