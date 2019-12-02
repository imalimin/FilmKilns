/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALIMAGELAYER_H
#define HWVC_ANDROID_ALIMAGELAYER_H

#include "Object.h"
#include "AlImageCanvas.h"
#include "HwAbsTexture.h"
#include "AlCanvasDrawer.h"
#include "AlImageLayerModel.h"
#include "AlLayerMeasure.h"

class AlImageLayerManager;

al_class(AlImageLayer) {
public:
public:
    static AlImageLayer *create(AlImageLayerModel *model, HwAbsTexture *tex);

public:
    ~AlImageLayer();

    void draw(AlImageCanvas *canvas);

    int32_t getWidth();

    int32_t getHeight();

private:
    AlImageLayer(AlImageLayerModel *model, HwAbsTexture *tex);

    AlImageLayer(const AlImageLayer &e) : Object() {};

    void _draw(AlImageCanvas *canvas);

    void _measure(AlSize &canvasSize);

private:
    AlImageLayerModel *model = nullptr;
    //Manager by TextureAllocator. DON`T delete here.
    friend AlImageLayerManager;
    HwAbsTexture *tex = nullptr;
    AlCanvasDrawer *mCanvasDrawer = nullptr;
    AlLayerMeasure measure;
};


#endif //HWVC_ANDROID_ALIMAGELAYER_H
