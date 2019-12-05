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

#define TAG "AlImageCanvas"

AlImageCanvas::AlImageCanvas() : Object() {

}

AlImageCanvas::~AlImageCanvas() {

}

void AlImageCanvas::release() {
    delete fbo;
    delete mBgDrawer;
    delete mCanvasDrawer;
    mCanvasDrawer = nullptr;
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
}

void AlImageCanvas::clear() {
    if (mCanvasTex && mBgDrawer) {
        glViewport(0, 0, mCanvasTex->getWidth(), mCanvasTex->getHeight());
        AlSize size(mCanvasTex->getWidth(), mCanvasTex->getHeight());
        mBgDrawer->update(size);
        mBgDrawer->draw(mCanvasTex);
        Logcat::i(TAG, "%s(%d) Canvas clear. Size %dx%d",
                  __FUNCTION__, __LINE__,
                  mCanvasTex->getWidth(), mCanvasTex->getHeight());
    }
}

int32_t AlImageCanvas::getWidth() {
    return nullptr != mCanvasTex ? mCanvasTex->getWidth() : 0;
}

int32_t AlImageCanvas::getHeight() {
    return nullptr != mCanvasTex ? mCanvasTex->getHeight() : 0;
}

HwResult AlImageCanvas::draw(AlImageLayer *layer) {
    if (nullptr == mCanvasDrawer) {
        mCanvasDrawer = new AlCanvasDrawer();
        mCanvasDrawer->prepare();
    }
    AlImageLayerDrawModel model(AlSize(getWidth(), getHeight()));
    if (mCanvasTex && layer && Hw::SUCCESS == layer->measure(model)) {
        _draw(model);
        return Hw::SUCCESS;
    }
    return Hw::FAILED;
}

void AlImageCanvas::_draw(AlImageLayerDrawModel &model) {
    mCanvasDrawer->setAlpha(model.alpha);
    ///设置Drawer的变换矩阵
    mCanvasDrawer->setMatrix(model.mat);
    ///设置纹理顶点
    mCanvasDrawer->setVertexRectF(model.vertexRectF);
    glViewport(0, 0, model.getCanvasSize().width, model.getCanvasSize().height);
    ///Draw layer
    mCanvasDrawer->draw(model.tex, mCanvasTex);
}

HwResult AlImageCanvas::read(AlBuffer *buf) {
    if (nullptr == mCanvasTex || nullptr == buf) return Hw::FAILED;
    fbo->bind();
    mCanvasTex->read(buf->data());
    fbo->unbind();
    return Hw::SUCCESS;
}
