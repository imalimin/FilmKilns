/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include <algorithm>
#include "AlImageLayerManager.h"
#include "HwBitmapFactory.h"
#include "Logcat.h"

AlImageLayerManager::AlImageLayerManager() : Object() {

}

AlImageLayerManager::~AlImageLayerManager() {
}

void AlImageLayerManager::release() {
    auto itr = mLayers.begin();
    while (mLayers.end() != itr) {
        AlImageLayer *layer = itr->second;
//            texAllocator->recycle(&(itr->second->tex));
        delete layer;
        ++itr;
    }
    mLayers.clear();
}

void AlImageLayerManager::update(std::vector<AlImageLayerModel *> *list,
                                 TextureAllocator *texAllocator) {
    this->models = list;
    std::vector<int32_t> ids;
    unsigned int size = list->size();
    for (unsigned int i = 0; i < size; ++i) {
        auto *it = list->at(i);
        ids.push_back(it->getId());
        if (!_found(it->getId())) {
            _newLayer(it, texAllocator);
        }
    }

    for (auto itr = mLayers.begin(); mLayers.end() != itr;) {
        auto result = std::find(ids.begin(), ids.end(), itr->first);
        if (result == ids.end()) {
            auto *it = itr->second;
            itr = mLayers.erase(itr);
            texAllocator->recycle(&(it->tex));
            delete it;
        } else {
            ++itr;
        }
    }
}

bool AlImageLayerManager::_newLayer(AlImageLayerModel *model,
                                    TextureAllocator *texAllocator) {
    HwBitmap *bmp = HwBitmapFactory::decodeFile(model->getPath());
    if (nullptr == bmp) {
        Logcat::e("AlImageLayerManager", "%s(%d): decode %s failed", __FUNCTION__, __LINE__,
                  model->getPath().c_str());
        return true;
    }
    auto *tex = texAllocator->alloc(bmp->getPixels(),
                                    bmp->getWidth(),
                                    bmp->getHeight(),
                                    GL_RGBA);
    delete bmp;
    AlImageLayer *layer = AlImageLayer::create(model, tex);
    mLayers.insert(pair<int32_t, AlImageLayer *>(model->getId(), layer));
    return true;
}

bool AlImageLayerManager::_found(int32_t id) {
    return mLayers.end() != mLayers.find(id);
}

int32_t AlImageLayerManager::size() {
    if (empty()) {
        return 0;
    }
    return models->size();
}

bool AlImageLayerManager::empty() {
    return nullptr == models || models->empty();
}

AlImageLayer *AlImageLayerManager::getLayer(int32_t index) {
    if (0 == size()) return nullptr;
    int32_t id = models->at(index)->getId();
    auto itr = mLayers.find(id);
    if (mLayers.end() == itr) {
        return nullptr;
    }
    return itr->second;
}
