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
#include "AlMosaicFilter.h"
#include "AlTexManager.h"

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
    if (mGridTex) {
        AlTexManager::instance()->recycle(&mGridTex);
        mGridTex = nullptr;
    }
#ifdef ENABLE_CROP_DEBUG
    delete mCopyDrawer;
    delete mAlQuadDrawer;
    mCopyDrawer = nullptr;
    mAlQuadDrawer = nullptr;
#endif
}

HwAbsTexture *AlImageCanvas::getOutput() {
    if (nullptr == mCanvasTex) return nullptr;
    return HwTexture::wrap(dynamic_cast<HwTexture *>(mCanvasTex));
}

void AlImageCanvas::update(int32_t w, int32_t h, int32_t color) {
    if(nullptr == paintFilter) {
        paintFilter = new AlPaintFilter();
        paintFilter->prepare();
        paintFilter->setColor(AlColor(0x0000ff00));
        paintFilter->setPaintSize(0.006f);
    }
    if (nullptr == mCanvasTex) {
        AlTexDescription desc;
        desc.size.width = w;
        desc.size.height = h;
        desc.fmt = GL_RGBA;
        mCanvasTex = AlTexManager::instance()->alloc(desc);
        fbo = HwFBObject::alloc();
        fbo->bindTex(mCanvasTex);
        mBgDrawer = AlColorGridFilter::create();
        mGridTex = AlTexManager::instance()->alloc();
        mBgDrawer->prepare(mGridTex);
#ifdef ENABLE_CROP_DEBUG
        AlTexDescription d;
        d.size.width = w;
        d.size.height = h;
        d.fmt = GL_RGBA;
        mLayerTex = texAllocator->alloc(d);
        mCopyDrawer = new HwNormalFilter();
        mCopyDrawer->prepare();
        mAlQuadDrawer = new AlQuadDrawer();
        mAlQuadDrawer->prepare(texAllocator);
#endif
    } else {
        mCanvasTex->update(nullptr, w, h, GL_RGBA);
    }
    clear();
}

void AlImageCanvas::clear(bool transparent) {
    if (mCanvasTex) {
        glViewport(0, 0, mCanvasTex->getWidth(), mCanvasTex->getHeight());
        if (!transparent && mBgDrawer) {
            mBgDrawer->draw(mCanvasTex);
        } else {
            fbo->bind();
            glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            fbo->unbind();
        }
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

HwResult AlImageCanvas::draw(AlImageLayerDrawModel *description) {
    if (nullptr == mCanvasDrawer) {
        mCanvasDrawer = new AlCanvasDrawer();
        mCanvasDrawer->prepare();
    }
    if (mCanvasTex && description) {
        _draw(description);
        return Hw::SUCCESS;
    }
    return Hw::FAILED;
}

void AlImageCanvas::_draw(AlImageLayerDrawModel *description) {
    mCanvasDrawer->setAlpha(description->alpha);
    ///设置Drawer的变换矩阵
    mCanvasDrawer->setMatrix(description->mat);
    ///设置纹理顶点
    mCanvasDrawer->setVertexRectF(description->vertexRectF);
#ifdef ENABLE_CROP_DEBUG
    glViewport(0, 0, description->getLayerSize().width, description->getLayerSize().height);
    mLayerTex->update(nullptr, description->getLayerSize().width,
                      description->getLayerSize().height, GL_RGBA);
    mCopyDrawer->draw(description->tex, mLayerTex);
    if (!description->cropQuad.isZero()) {
        mAlQuadDrawer->setQuad(description->cropQuad);
        mAlQuadDrawer->draw(mLayerTex);
    }
    glViewport(0, 0, getWidth(), getHeight());
    mCanvasDrawer->draw(mLayerTex, mCanvasTex);
#else
    mCanvasDrawer->setPositionQuad(description->cropQuad);
    glViewport(0, 0, getWidth(), getHeight());
    ///Draw layer
    mCanvasDrawer->draw(description->tex, mCanvasTex);
#endif
    _drawDebug();
}

HwResult AlImageCanvas::read(AlBuffer *buf) {
    if (nullptr == mCanvasTex || nullptr == buf) return Hw::FAILED;
    glFinish();
    fbo->bind();
    mCanvasTex->read(buf->data());
    fbo->unbind();
    return Hw::SUCCESS;
}

void AlImageCanvas::_drawDebug() {
    std::vector<float> point(2);
    point[0] = 0.25f;
    point[1] = 0.55f;
    paintFilter->setPath(&point, true);
    paintFilter->draw(mCanvasTex, mCanvasTex);
}
