/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALIMAGELAYER_H
#define HWVC_ANDROID_ALIMAGELAYER_H

#include "Object.h"
#include "HwResult.h"
#include "HwAbsTexture.h"
#include "AlImageLayerModel.h"

class AlImageCanvas;

class AlImageLayerManager;

al_class(AlImageLayer) {
public:
public:
    static AlImageLayer *create(AlImageLayerModel *model, HwAbsTexture *tex);

public:
    AlImageLayerModel *model = nullptr;

    ~AlImageLayer();

    int32_t getWidth();

    int32_t getHeight();

    HwAbsTexture *getTexture();

private:
    AlImageLayer(AlImageLayerModel *model, HwAbsTexture *tex);

    AlImageLayer(const AlImageLayer &e) : Object() {};

private:
    /// Manager by AlTexAllocator. DON`T delete here.
    friend AlImageLayerManager;
    HwAbsTexture *tex = nullptr;
};


#endif //HWVC_ANDROID_ALIMAGELAYER_H
