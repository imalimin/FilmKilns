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

AlImageCanvas::AlImageCanvas() : Object(), mBGType(kBGType::NONE) {

}

AlImageCanvas::~AlImageCanvas() {

}

void AlImageCanvas::release() {
    delete fbo;
    delete mBgDrawer;
    delete mCanvasDrawer;
    AlTexManager::instance()->recycle(&mCanvasTex);
    delete paintFilter;
    paintFilter = nullptr;
    mCanvasDrawer = nullptr;
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
    if (nullptr == paintFilter) {
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
        mBgDrawer->prepare();
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

void AlImageCanvas::clear(bool retry) {
    if (mCanvasTex) {
        AlColor color(0x00000000);
        if (kBGType::BLACK == mBGType) {
            color = AlColor(0x00000000);
        } else if (kBGType::WHITE == mBGType) {
            color = AlColor(0x00ffffff);
        }
        glViewport(0, 0, mCanvasTex->getWidth(), mCanvasTex->getHeight());
        if (kBGType::GRID == mBGType && mBgDrawer) {
            mBgDrawer->draw(mCanvasTex);
        } else {
            fbo->bind();
            glClearColor(color.rf(), color.gf(), color.bf(), color.af());
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            fbo->unbind();
            if (GL_NO_ERROR != glGetError() && retry) {
                AlLogW(TAG, "Retry to clear");
                if (fbo) {
                    delete fbo;
                    fbo = HwFBObject::alloc();
                    AlLogW(TAG, "Retry to alloc fbo, error code %d", glGetError());
                    fbo->bindTex(mCanvasTex);
                }
                clear(false);
            }
        }
//        AlLogD(TAG, "Size %dx%d", mCanvasTex->getWidth(), mCanvasTex->getHeight());
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
    ///设置纹理顶点
    mCanvasDrawer->setVertexQuad(description->vertex);
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
//    _drawDebug();
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
    std::vector<float> point(4);
    point[0] = 0.f;
    point[1] = 0.f;
    point[2] = 0.25f;
    point[3] = 0.55f;
    paintFilter->setPath(&point, true);
    paintFilter->draw(mCanvasTex, mCanvasTex);
}

void AlImageCanvas::setBGType(AlImageCanvas::kBGType type) {
    this->mBGType = type;
}
