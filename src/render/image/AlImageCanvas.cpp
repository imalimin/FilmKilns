/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlImageCanvas.h"
#include "HwFBObject.h"
#include "HwTexture.h"
#include "Logcat.h"

AlImageCanvas::AlImageCanvas() : Object() {

}

AlImageCanvas::~AlImageCanvas() {

}

void AlImageCanvas::release() {
    delete fbo;
    delete mBgDrawer;
    mCanvasTex = nullptr;
}

HwAbsTexture *AlImageCanvas::getOutput() {
    if (nullptr == mCanvasTex) return nullptr;
    return HwTexture::wrap(mCanvasTex->target(), mCanvasTex->texId(),
                           mCanvasTex->getWidth(),
                           mCanvasTex->getHeight(),
                           mCanvasTex->fmt());
}

void AlImageCanvas::update(int32_t w, int32_t h, int32_t color, TextureAllocator *texAllocator) {
    if (nullptr == mCanvasTex) {
        mCanvasTex = texAllocator->alloc(nullptr, w, h, GL_RGBA);
        fbo = HwFBObject::alloc();
        fbo->bindTex(mCanvasTex);
        mBgDrawer = AlColorGridFilter::create();
        mBgDrawer->prepare(texAllocator);
    } else {
        mCanvasTex->update(nullptr, w, h, GL_RGBA);
    }
    clear();
    Logcat::e("AlImageCanvas", "%s(%d): Canvas size %dx%d", __FUNCTION__, __LINE__,
              mCanvasTex->getWidth(), mCanvasTex->getHeight());
}

void AlImageCanvas::clear() {
    if (mCanvasTex && mBgDrawer) {
        glViewport(0, 0, mCanvasTex->getWidth(), mCanvasTex->getHeight());
        mBgDrawer->draw(mCanvasTex);
    }
}

int32_t AlImageCanvas::getWidth() {
    return nullptr != mCanvasTex ? mCanvasTex->getWidth() : 0;
}

int32_t AlImageCanvas::getHeight() {
    return nullptr != mCanvasTex ? mCanvasTex->getHeight() : 0;
}
