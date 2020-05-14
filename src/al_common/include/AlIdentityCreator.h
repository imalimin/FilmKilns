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

al_class AlIdentityCreator al_extend Object {
public:
    static constexpr int32_t NONE_ID = -1;

    AlIdentityCreator();

    virtual ~AlIdentityCreator();

    int32_t generate();

    void reset(int32_t cur = 0);

private:
    AlIdentityCreator(const AlIdentityCreator &o);

private:
    int32_t cur = NONE_ID;
    std::mutex mtx;
};


#endif //HWVC_ANDROID_ALIDENTITYCREATOR_H
