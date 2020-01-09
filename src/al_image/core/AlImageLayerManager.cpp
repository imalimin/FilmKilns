/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include <algorithm>
#include "AlImageLayerManager.h"
#include "AlBitmapFactory.h"
#include "AlRotateFilter.h"
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
                                 AlTexAllocator *texAllocator) {
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
                                    AlTexAllocator *texAllocator) {
    AlBitmap *bmp = AlBitmapFactory::decodeFile(model->getPath());
    if (nullptr == bmp) {
        Logcat::e("AlImageLayerManager", "%s(%d): decode %s failed", __FUNCTION__, __LINE__,
                  model->getPath().c_str());
        return true;
    }
    auto rotation = bmp->getRotation();
    AlTexDescription desc;
    desc.size.width = bmp->getWidth();
    desc.size.height = bmp->getHeight();
    desc.wrapMode = AlTexDescription::WrapMode::BORDER;
    desc.fmt = GL_RGBA;
    AlBuffer *buf = AlBuffer::wrap(bmp->getPixels(), bmp->getByteSize());
    auto *srcTex = texAllocator->alloc(desc, buf);
    delete buf;
    delete bmp;
    _correctAngle(texAllocator, &srcTex, rotation);
    AlImageLayer *layer = AlImageLayer::create(srcTex);
    mLayers.insert(pair<int32_t, AlImageLayer *>(model->getId(), layer));
    return true;
}

void AlImageLayerManager::_correctAngle(AlTexAllocator *texAllocator,
                                        HwAbsTexture **tex,
                                        AlRational radian) {
    HwAbsTexture *destTex = nullptr;
    auto rFloat = fmod(std::abs(radian.toFloat()), 2.0);
    if (0 != rFloat) {
        AlTexDescription desc;
        desc.fmt = (*tex)->fmt();
        if (0.5 == rFloat || 1.5 == rFloat) {///宽高对换
            desc.size.width = (*tex)->getHeight();
            desc.size.height = (*tex)->getWidth();
        } else {
            desc.size.width = (*tex)->getWidth();
            desc.size.height = (*tex)->getHeight();
        }
        destTex = texAllocator->alloc(desc);
        AlRotateFilter filter;
        filter.prepare();
        filter.setRotation(radian);
        glViewport(0, 0, destTex->getWidth(), destTex->getHeight());
        filter.draw(*tex, destTex);
        texAllocator->recycle(tex);
        *tex = destTex;
    }
}

bool AlImageLayerManager::_found(int32_t id) {
    return mLayers.end() != mLayers.find(id);
}

size_t AlImageLayerManager::size() {
    if (empty()) {
        return 0;
    }
    return models->size();
}

bool AlImageLayerManager::empty() {
    return nullptr == models || models->empty();
}

AlImageLayerModel *AlImageLayerManager::getLayer(int32_t index) {
    if (empty()) return nullptr;
    return models->at(index);
}

AlImageLayer *AlImageLayerManager::find(int32_t id) {
    if (empty()) return nullptr;
    auto itr = mLayers.find(id);
    if (mLayers.end() == itr) {
        return nullptr;
    }
    return itr->second;
}

void AlImageLayerManager::replaceAll(AlTexAllocator *texAllocator,
                                     std::vector<AlImageLayerModel *> *list) {
    if (nullptr == list || list->empty()) {
        return;
    }
    ///Clear all layer model.
    size_t size = models->size();
    for (int i = 0; i < size; ++i) {
        AlImageLayerModel *it = (*models)[i];
        delete it;
    }
    models->clear();
    ///Clear all layer.
    auto itr = mLayers.begin();
    while (mLayers.end() != itr) {
        AlImageLayer *layer = itr->second;
        texAllocator->recycle(&(itr->second->tex));
        delete layer;
        ++itr;
    }
    mLayers.clear();
    ///Replace layer model.
    size = list->size();
    for (int i = 0; i < size; ++i) {
        models->push_back((*list)[i]);
    }
    ///Update layer model.
    update(models, texAllocator);
}

int32_t AlImageLayerManager::getMaxId() {
    if (empty()) return 0;
    int32_t id = 0;
    size_t size = models->size();
    for (int i = 0; i < size; ++i) {
        int32_t tmp = (*models)[i]->getId();
        id = max(tmp, id);
    }
    return id;
}
