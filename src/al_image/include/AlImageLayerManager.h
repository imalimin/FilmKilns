/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALIMAGELAYERMANAGER_H
#define HWVC_ANDROID_ALIMAGELAYERMANAGER_H

#include "Object.h"
#include "AlImageLayer.h"
#include "AlImageLayerModel.h"
#include "AlTexAllocator.h"
#include <vector>
#include <map>

al_class(AlImageLayerManager) {
public:
    AlImageLayerManager();

    ~AlImageLayerManager();

    void release();

    void update(std::vector<AlImageLayerModel *> *list, AlTexAllocator *texAllocator);

    int32_t size();

    bool empty();

    AlImageLayer *getLayer(int32_t index);

    void replaceAll(AlTexAllocator *texAllocator, std::vector<AlImageLayerModel *> *list);

    int32_t getMaxId();

private:
    AlImageLayerManager(const AlImageLayerManager &e) : Object() {};

    bool _newLayer(AlImageLayerModel *model, AlTexAllocator *texAllocator);

    bool _found(int32_t id);

    void _correctAngle(AlTexAllocator *texAllocator, HwAbsTexture **tex, AlRational radian);

private:
    /**
     * AlImageProcessor::mLayers instance.
     */
    std::vector<AlImageLayerModel *> *models = nullptr;
    std::map<int32_t, AlImageLayer *> mLayers;
};


#endif //HWVC_ANDROID_ALIMAGELAYERMANAGER_H
