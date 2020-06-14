/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALIDENTITYCREATOR_H
#define HWVC_ANDROID_ALIDENTITYCREATOR_H

#include "Object.h"
#include <mutex>

typedef int32_t AlID;

AL_CLASS AlIdentityCreator AL_EXTEND Object {
public:
    static constexpr AlID NONE_ID = -1;

    AlIdentityCreator();

    virtual ~AlIdentityCreator();

    AlID generate();

    void reset(int32_t cur = 0);

private:
    AlIdentityCreator(const AlIdentityCreator &o);

private:
    int32_t cur = NONE_ID;
    std::mutex mtx;
};


#endif //HWVC_ANDROID_ALIDENTITYCREATOR_H
