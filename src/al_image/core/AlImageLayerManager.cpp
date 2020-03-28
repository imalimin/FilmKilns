/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include <algorithm>
#include "AlImageLayerManager.h"
#include "AlTexManager.h"
#include "Logcat.h"

#define TAG "AlImageLayerManager"

AlImageLayerManager::AlImageLayerManager() : Object() {

}

AlImageLayerManager::~AlImageLayerManager() {
}

void AlImageLayerManager::clear() {
    auto itr = mLayers.begin();
    while (mLayers.end() != itr) {
        AlImageLayer *layer = itr->second;
        AlTexManager::instance()->recycle(&(layer->tex));
        delete layer;
        ++itr;
    }
    mLayers.clear();
    size_t size = models.size();
    for (int i = 0; i < size; ++i) {
        AlImageLayerModel *it = models[i];
        delete it;
    }
    models.clear();
}

int32_t AlImageLayerManager::addLayer(HwAbsTexture *tex, const std::string path) {
    if (nullptr == tex) {
        AlLogE(TAG, "failed %d", 0);
        return AlIdentityCreator::NONE_ID;
    }
    auto *model = AlImageLayerModel::create(&mLayerIdCreator, path);
    AlImageLayer *layer = AlImageLayer::create(tex);
    models.push_back(model);
    mLayers.insert(pair<int32_t, AlImageLayer *>(model->getId(), layer));
    return model->getId();
}

int32_t AlImageLayerManager::addLayer(HwAbsTexture *tex, AlImageLayerModel &model) {
    if (nullptr == tex) {
        AlLogE(TAG, "failed %d", 0);
        return AlIdentityCreator::NONE_ID;
    }
    auto *m = AlImageLayerModel::create(&mLayerIdCreator, model);
    AlImageLayer *layer = AlImageLayer::create(tex);
    models.push_back(m);
    mLayers.insert(pair<int32_t, AlImageLayer *>(m->getId(), layer));
    return m->getId();
}

void AlImageLayerManager::removeLayer(const int32_t id) {
    for (auto itr = models.begin(); itr != models.end(); ++itr) {
        auto *model = *itr;
        if (model && model->getId() == id) {
            models.erase(itr);
            auto it = mLayers.find(model->getId());
            if (it != mLayers.end()) {
                auto *layer = it->second;
                mLayers.erase(it);
                _delLayer(layer);
            }
            delete model;
            break;
        }
    }
}

void AlImageLayerManager::update(std::vector<int32_t> *delLayers) {
    std::vector<int32_t> ids;
    for (auto *it : models) {
        ids.push_back(it->getId());
    }

    for (auto itr = mLayers.begin(); mLayers.end() != itr;) {
        auto result = std::find(ids.begin(), ids.end(), itr->first);
        if (result == ids.end()) {
            auto *it = itr->second;
            itr = mLayers.erase(itr);
            AlTexManager::instance()->recycle(&(it->tex));
            if (delLayers) {
                delLayers->emplace_back(itr->first);
            }
            delete it;
        } else {
            ++itr;
        }
    }
}

size_t AlImageLayerManager::size() {
    if (empty()) {
        return 0;
    }
    return models.size();
}

bool AlImageLayerManager::empty() {
    return models.empty();
}

AlImageLayerModel *AlImageLayerManager::getLayer(int32_t index) {
    if (empty()) return nullptr;
    return models[index];
}

AlImageLayer *AlImageLayerManager::find(int32_t id) {
    if (empty()) return nullptr;
    auto itr = mLayers.find(id);
    if (mLayers.end() == itr) {
        return nullptr;
    }
    return itr->second;
}

AlImageLayerModel *AlImageLayerManager::findModel(float x, float y) {
    size_t size = models.size();
    for (int i = size - 1; i >= 0; --i) {
        auto *m = models[i];
        if (m && m->getQuad().inside(AlPointF(x, y))) {
            return m;
        }
    }
    return nullptr;
}

AlImageLayerModel *AlImageLayerManager::findModel(const int32_t id) {
    for (auto *m:models) {
        if (m && m->getId() == id) {
            return m;
        }
    }
    return nullptr;
}

void AlImageLayerManager::_delLayer(AlImageLayer *&layer) {
    if (layer) {
        AlTexManager::instance()->recycle(&(layer->tex));
        delete layer;
    }
}

AlImageLayerModel *AlImageLayerManager::findModelByIndex(const int32_t idx) {
    return models[idx];
}
