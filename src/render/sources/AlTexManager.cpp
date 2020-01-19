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
        Logcat::e(TAG, "%s(%d) failed", __FUNCTION__, __LINE__);
        return nullptr;
    }
    std::lock_guard<std::mutex> guard(mtx);
    auto *allocator = _find(looper);
    auto *tex = allocator->alloc();
    dump();
    return tex;
}

HwAbsTexture *AlTexManager::alloc(AlTexDescription &desc, AlBuffer *buf) {
    int64_t looper;
    if (!_checkEnv(looper)) {
        Logcat::e(TAG, "%s(%d) failed", __FUNCTION__, __LINE__);
        return nullptr;
    }
    std::lock_guard<std::mutex> guard(mtx);
    auto *allocator = _find(looper);
    auto *tex = allocator->alloc(desc, buf);
    dump();
    return tex;
}

bool AlTexManager::recycle(HwAbsTexture **tex) {
    int64_t looper;
    if (!_checkEnv(looper)) {
        Logcat::e(TAG, "%s(%d) failed", __FUNCTION__, __LINE__);
        return false;
    }
    std::lock_guard<std::mutex> guard(mtx);
    auto *allocator = _find(looper);
    if (!allocator->recycle(tex)) {
        Logcat::w(TAG, "%s(%d) warning", __FUNCTION__, __LINE__);
        return false;
    }
    if (allocator->empty()) {
        auto itr = map.find(looper);
        delete itr->second;
        map.erase(itr);
        Logcat::i(TAG, "%s(%d) remove allocator(%p)", __FUNCTION__, __LINE__, itr->second);
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
        Logcat::e(TAG, "%s(%d) failed. looper=%p", __FUNCTION__, __LINE__, l);
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
    Logcat::i(TAG, "%s(%d) countOfByte=%lld, countOfTex=%d",
              __FUNCTION__, __LINE__,
              countOfByte, countOfTex);
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
