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
#include "AlLayerMeasure.h"
#include "AlImageLayerDrawModel.h"

class AlImageCanvas;

class AlImageLayerManager;

al_class(AlImageLayer) {
public:
public:
    static AlImageLayer *create(AlImageLayerModel *model, HwAbsTexture *tex);

public:
    ~AlImageLayer();

    int32_t getWidth();

    int32_t getHeight();

private:
    AlImageLayer(AlImageLayerModel *model, HwAbsTexture *tex);

    AlImageLayer(const AlImageLayer &e) : Object() {};

private:
    friend AlImageCanvas;

    /// Measure all params for canvas
    /// \param drawModel
    /// \return {@link HwResult}
    HwResult measure(AlImageLayerDrawModel &drawModel);

private:
    AlImageLayerModel *model = nullptr;
    //Manager by TextureAllocator. DON`T delete here.
    friend AlImageLayerManager;
    HwAbsTexture *tex = nullptr;
    AlLayerMeasure aMeasure;
};


#endif //HWVC_ANDROID_ALIMAGELAYER_H
