/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALLAYERPAIR_H
#define HWVC_ANDROID_ALLAYERPAIR_H

#include "AlImageLayer.h"
#include "AlImageLayerModel.h"

al_class AlLayerPair al_extend Object {
public:
    AlLayerPair(AlImageLayer *layer, AlImageLayerModel *model);

    AlLayerPair(const AlLayerPair &o);

    virtual ~AlLayerPair();

public:
    AlImageLayer *layer = nullptr;
    AlImageLayerModel *model = nullptr;
};


#endif //HWVC_ANDROID_ALLAYERPAIR_H
