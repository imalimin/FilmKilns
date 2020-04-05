/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlTexManager.h"
#include "AlLooper.h"
#include "AlEgl.h"

#define TAG "AlTexManager"

AlTexManager *AlTexManager::_instance = new AlTexManager();

AlTexManager *AlTexManager::instance() {
    return _instance;
}

HwAbsTexture *AlTexManager::alloc() {
    int64_t looper;
    if (!_checkEnv(looper)) {
        AlLogI(TAG, "failed.");
        return nullptr;
    }
    std::lock_guard<std::mutex> guard(mtx);
    auto *allocator = _find(looper);
    auto *tex = allocator->alloc();
    AlLogI(TAG, "alloc %d", tex->texId());
    dump();
    return tex;
}

HwAbsTexture *AlTexManager::alloc(AlTexDescription &desc, AlBuffer *buf) {
    int64_t looper;
    if (!_checkEnv(looper)) {
        AlLogI(TAG, "failed.");
        return nullptr;
    }
    std::lock_guard<std::mutex> guard(mtx);
    auto *allocator = _find(looper);
    auto *tex = allocator->alloc(desc, buf);
    AlLogI(TAG, "alloc %d", tex->texId());
    dump();
    return tex;
}

bool AlTexManager::recycle(HwAbsTexture **tex) {
    int64_t looper;
    if (nullptr == tex || nullptr == *tex || !_checkEnv(looper)) {
        AlLogE(TAG, "failed");
        return false;
    }
    std::lock_guard<std::mutex> guard(mtx);
    auto *allocator = _find(looper);
    int id = (*tex)->texId();
    if (!allocator->recycle(tex)) {
        AlLogW(TAG, "failed");
        return false;
    }
    AlLogI(TAG, "recycle %d", id);
    if (allocator->empty()) {
        auto itr = map.find(looper);
        AlLogI(TAG, "remove allocator(%p)", itr->second);
        delete itr->second;
        map.erase(itr);
    }
    dump();
    return true;
}

AlTexAllocator *AlTexManager::_find(int64_t looper) {
    auto itr = map.find(looper);
    if (map.end() == itr) {
        auto *allocator = new AlTexAllocator();
        map.insert(pair<int64_t, AlTexAllocator *>(looper, allocator));
        return allocator;
    }
    return itr->second;
}

bool AlTexManager::_checkEnv(int64_t &looper) {
    auto *l = AlLooper::myLooper();
    if (nullptr == l || EGL_NO_CONTEXT == AlEgl::currentContext()) {
        AlLogI(TAG, "failed. looper=%p", l);
        looper = 0;
        return false;
    }
    looper = reinterpret_cast<int64_t>(l);
    return true;
}

void AlTexManager::dump() {
    int32_t countOfTex = 0;
    int64_t countOfByte = 0;
    count(countOfTex, countOfByte);
    AlLogI(TAG, "countOfByte=%"
            PRId64
            ", countOfTex=%d", countOfByte, countOfTex);
}

void AlTexManager::count(int32_t &countOfTex, int64_t &countOfByte) {
    countOfTex = 0;
    countOfByte = 0;
    auto itr = map.begin();
    while (map.end() != itr) {
        countOfTex += itr->second->countOfTex();
        countOfByte += itr->second->countOfByte();
        ++itr;
    }
}
