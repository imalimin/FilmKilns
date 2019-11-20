/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlImageCanvas.h"
#include "HwFBObject.h"
#include "HwTexture.h"

AlImageCanvas::AlImageCanvas() : Object() {

}

AlImageCanvas::~AlImageCanvas() {

}

void AlImageCanvas::prepare(TextureAllocator *texAllocator) {
    this->texAllocator = texAllocator;
}

void AlImageCanvas::release() {
    delete mCanvasTex;
    delete fbo;
}

HwAbsTexture *AlImageCanvas::getOutput() {
    return HwTexture::wrap(mCanvasTex->target(), mCanvasTex->texId(),
                           mCanvasTex->getWidth(),
                           mCanvasTex->getHeight(),
                           mCanvasTex->fmt());
}

void AlImageCanvas::update(int32_t w, int32_t h, int32_t color) {
    if (nullptr == mCanvasTex) {
        mCanvasTex = texAllocator->alloc(nullptr, w, h, GL_RGBA);
        fbo = HwFBObject::alloc();
        fbo->bindTex(mCanvasTex);
    } else {
        mCanvasTex->update(nullptr, w, h, GL_RGBA);
    }
    fbo->bind();
    glClearColor(0.5, 0.2, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    fbo->unbind();
}
