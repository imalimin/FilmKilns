/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALTEXMANAGER_H
#define HWVC_ANDROID_ALTEXMANAGER_H

#include "Object.h"
#include "HwAbsTexture.h"
#include "AlTexAllocator.h"
#include <map>
#include <mutex>

al_class AlTexManager al_extend Object {
public:
    static AlTexManager *instance();

    HwAbsTexture *alloc();

    HwAbsTexture *alloc(AlTexDescription &desc, AlBuffer *buf = nullptr);

    bool recycle(HwAbsTexture **tex);

    void count(int32_t &countOfTex, int64_t &countOfByte);

private:
    AlTexManager() : Object() {};

    AlTexManager(const AlTexManager &o) : Object() {};

    ~AlTexManager() {};

    AlTexAllocator *_find(int64_t looper);

    bool _checkEnv(int64_t &looper);

    void dump();

private:
    static AlTexManager *_instance;
    std::map<int64_t, AlTexAllocator *> map;
    std::mutex mtx;
};


#endif //HWVC_ANDROID_ALTEXMANAGER_H
