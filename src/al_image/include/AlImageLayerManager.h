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
#include <vector>
#include <map>

al_class(AlImageLayerManager) {
public:
    AlImageLayerManager();

    ~AlImageLayerManager();

    void release();

    int32_t addLayer(const std::string path);

    void removeLayer(const int32_t id);

    void update(std::vector<int32_t> *delLayers = nullptr);

    size_t size();

    bool empty();

    AlImageLayerModel *getLayer(int32_t index);

    AlImageLayerModel *findModel(float x, float y);

    AlImageLayerModel *findModel(const int32_t id);

    AlImageLayerModel *findModelByIndex(const int32_t idx);

    AlImageLayer *find(int32_t id);

    void replaceAll(std::vector<AlImageLayerModel *> *list);

    int32_t getMaxId();

private:
    AlImageLayerManager(const AlImageLayerManager &e) : Object() {};

    void _delLayer(AlImageLayer *&layer);

    void _correctAngle(HwAbsTexture **tex, AlRational radian);

private:
    AlIdentityCreator mLayerIdCreator;
    /**
     * AlImageProcessor::mLayers instance.
     */
    std::vector<AlImageLayerModel *> models;
    std::map<int32_t, AlImageLayer *> mLayers;
};


#endif //HWVC_ANDROID_ALIMAGELAYERMANAGER_H
