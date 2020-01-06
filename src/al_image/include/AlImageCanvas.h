/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALIMAGECANVAS_H
#define HWVC_ANDROID_ALIMAGECANVAS_H

#include "Object.h"
#include "HwResult.h"
#include "AlBuffer.h"
#include "HwAbsTexture.h"
#include "AlImageLayer.h"
#include "AlCanvasDrawer.h"
#include "AlTexAllocator.h"
#include "AlColorGridFilter.h"
#include "AlImageLayerDrawModel.h"
#ifdef ENABLE_CROP_DEBUG
#include "AlQuadDrawer.h"
#include "HwNormalFilter.h"
#endif

al_class(AlImageCanvas) {
public:
    AlImageCanvas();

    ~AlImageCanvas();

    void release();

    void update(int32_t w, int32_t h, int32_t color, AlTexAllocator *texAllocator);

    void clear(bool transparent = false);

    HwAbsTexture *getOutput();

    int32_t getWidth();

    int32_t getHeight();

    HwResult draw(AlImageLayerDrawModel *description);

    HwResult read(AlBuffer *buf);

private:
    AlImageCanvas(const AlImageCanvas &e) : Object() {};

    void _draw(AlImageLayerDrawModel *description);

private:
    HwAbsFBObject *fbo = nullptr;
    /// Manager by AlTexAllocator. DON`T delete here.
    HwAbsTexture *mCanvasTex = nullptr;
    AlColorGridFilter *mBgDrawer = nullptr;
    AlCanvasDrawer *mCanvasDrawer = nullptr;
    HwAbsTexture *mFilterTex = nullptr;
    HwAbsFilter *mosaicFilter = nullptr;
#ifdef ENABLE_CROP_DEBUG
    HwAbsTexture *mLayerTex = nullptr;
    HwNormalFilter *mCopyDrawer = nullptr;
    AlQuadDrawer *mAlQuadDrawer = nullptr;
#endif
};


#endif //HWVC_ANDROID_ALIMAGECANVAS_H
