/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

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
        delete layer;
        ++itr;
    }
    mLayers.clear();
}

void AlImageLayerManager::update(std::list<AlImageLayerModel *> *list,
                                 TextureAllocator *texAllocator) {
    auto it = list->begin();
    while (list->end() != it) {
        if (!_found(*it)) {
            _newLayer(*it, texAllocator);
        }
        ++it;
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
    AlImageLayer *layer = AlImageLayer::create(tex);
    mLayers.insert(pair<AlImageLayerModel *, AlImageLayer *>(model, layer));
    return true;
}

bool AlImageLayerManager::_found(AlImageLayerModel *model) {
    return mLayers.end() != mLayers.find(model);
}

int32_t AlImageLayerManager::size() {
    return mLayers.size();
}

bool AlImageLayerManager::empty() {
    return 0 == size();
}

AlImageLayer *AlImageLayerManager::getLayout(int32_t index) {
    if (0 == size()) return nullptr;
    auto it = mLayers.begin();
    return it->second;
}
