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
#include "TextureAllocator.h"
#include "AlColorGridFilter.h"
#include "AlImageLayerDrawModel.h"

al_class(AlImageCanvas) {
public:
    AlImageCanvas();

    ~AlImageCanvas();

    void release();

    void update(int32_t w, int32_t h, int32_t color, TextureAllocator *texAllocator);

    void clear(bool transparent = false);

    HwAbsTexture *getOutput();

    int32_t getWidth();

    int32_t getHeight();

    HwResult draw(AlImageLayer *layer);

    HwResult read(AlBuffer *buf);

private:
    AlImageCanvas(const AlImageCanvas &e) : Object() {};

    void _draw(AlImageLayerDrawModel &model);

private:
    HwAbsFBObject *fbo = nullptr;
    //Manager by TextureAllocator. DON`T delete here.
    HwAbsTexture *mCanvasTex = nullptr;
    AlColorGridFilter *mBgDrawer = nullptr;
    AlCanvasDrawer *mCanvasDrawer = nullptr;
};


#endif //HWVC_ANDROID_ALIMAGECANVAS_H
