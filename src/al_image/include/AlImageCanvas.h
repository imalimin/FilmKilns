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
#include "AlColorGridFilter.h"
#include "AlImageLayerDrawModel.h"
#include "AlPaintFilter.h"

#ifdef ENABLE_CROP_DEBUG
#include "AlQuadDrawer.h"
#include "HwNormalFilter.h"
#endif

AL_CLASS AlImageCanvas AL_EXTEND Object {
public:
    AL_ENUM kBGType : int {
        NONE = 0,
        BLACK,
        WHITE,
        GRID,
    };

public:
    AlImageCanvas();

    ~AlImageCanvas();

    void release();

    void update(int32_t w, int32_t h, int32_t color);

    void clear(bool transparent = false);

    HwAbsTexture *getOutput();

    int32_t getWidth();

    int32_t getHeight();

    HwResult draw(AlImageLayerDrawModel *description);

    HwResult read(AlBuffer *buf);

    void setBGType(AlImageCanvas::kBGType type);

private:
    AlImageCanvas(const AlImageCanvas &e) : Object() {};

    void _draw(AlImageLayerDrawModel *description);

    void _drawDebug();

private:
    kBGType mBGType;
    HwAbsFBObject *fbo = nullptr;
    /// Manager by AlTexAllocator. DON`T delete here.
    HwAbsTexture *mCanvasTex = nullptr;
    AlColorGridFilter *mBgDrawer = nullptr;
    AlCanvasDrawer *mCanvasDrawer = nullptr;
    AlPaintFilter *paintFilter = nullptr;
#ifdef ENABLE_CROP_DEBUG
    HwAbsTexture *mLayerTex = nullptr;
    HwNormalFilter *mCopyDrawer = nullptr;
    AlQuadDrawer *mAlQuadDrawer = nullptr;
#endif
};


#endif //HWVC_ANDROID_ALIMAGECANVAS_H
