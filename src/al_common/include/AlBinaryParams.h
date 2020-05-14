/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALBINARYPARAMS_H
#define HWVC_ANDROID_ALBINARYPARAMS_H

#include "Object.h"

al_class AlBinaryParams al_extend Object {
public:
    AlBinaryParams(int32_t flags = 0);

    AlBinaryParams(const AlBinaryParams &o);

    virtual  ~AlBinaryParams();

    int32_t toInt();

    void set(int32_t index, bool value);

    bool get(int32_t index);

private:
    int32_t flags = 0;
};


#endif //HWVC_ANDROID_ALBINARYPARAMS_H
