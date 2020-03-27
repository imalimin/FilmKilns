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
#include "AlTexManager.h"
#include "Logcat.h"

#define TAG "AlImageLayerManager"

AlImageLayerManager::AlImageLayerManager() : Object() {

}

AlImageLayerManager::~AlImageLayerManager() {
}

void AlImageLayerManager::release() {
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
    mLayers.clear();
}

int32_t AlImageLayerManager::addLayer(const std::string path) {
    auto *model = AlImageLayerModel::create(&mLayerIdCreator, path);
    AlBitmap *bmp = AlBitmapFactory::decodeFile(model->getPath());
    if (nullptr == bmp || nullptr == model) {
        AlLogE(TAG, "decode %s failed", path.c_str());
        return AlIdentityCreator::NONE_ID;
    }
    auto rotation = bmp->getRotation();
    AlTexDescription desc;
    desc.size.width = bmp->getWidth();
    desc.size.height = bmp->getHeight();
    desc.wrapMode = AlTexDescription::WrapMode::BORDER;
    desc.fmt = GL_RGBA;
    AlBuffer *buf = AlBuffer::wrap(bmp->getPixels(), bmp->getByteSize());
    auto *srcTex = AlTexManager::instance()->alloc(desc, buf);
    delete buf;
    delete bmp;
    _correctAngle(&srcTex, rotation);
    AlImageLayer *layer = AlImageLayer::create(srcTex);
    models.push_back(model);
    mLayers.insert(pair<int32_t, AlImageLayer *>(model->getId(), layer));
    return model->getId();
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

void AlImageLayerManager::_correctAngle(HwAbsTexture **tex,
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
        destTex = AlTexManager::instance()->alloc(desc);
        AlRotateFilter filter;
        filter.prepare();
        filter.setRotation(radian);
        glViewport(0, 0, destTex->getWidth(), destTex->getHeight());
        filter.draw(*tex, destTex);
        AlTexManager::instance()->recycle(tex);
        *tex = destTex;
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

void AlImageLayerManager::replaceAll(std::vector<AlImageLayerModel *> *list) {
//    if (nullptr == list || list->empty()) {
//        return;
//    }
//    ///Clear all layer model.
//    for (auto *it : models) {
//        delete it;
//    }
//    models.clear();
//    ///Clear all layer.
//    auto itr = mLayers.begin();
//    while (mLayers.end() != itr) {
//        AlImageLayer *layer = itr->second;
//        AlTexManager::instance()->recycle(&(itr->second->tex));
//        delete layer;
//        ++itr;
//    }
//    mLayers.clear();
//    ///Replace layer model.
//    size_t size = list->size();
//    for (int i = 0; i < size; ++i) {
//        models.push_back((*list)[i]);
//    }
}

int32_t AlImageLayerManager::getMaxId() {
    if (empty()) return 0;
    int32_t id = 0;
    size_t size = models.size();
    for (int i = 0; i < size; ++i) {
        int32_t tmp = models[i]->getId();
        id = max(tmp, id);
    }
    return id;
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
