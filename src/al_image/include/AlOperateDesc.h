/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALOPERATEDESC_H
#define HWVC_ANDROID_ALOPERATEDESC_H

#include "Object.h"

al_class(AlOperateDesc) {
public:
    AlOperateDesc(int32_t layerId);

    AlOperateDesc(const AlOperateDesc &o);

    virtual ~AlOperateDesc();

public:
    int32_t layerId;
};


#endif //HWVC_ANDROID_ALOPERATEDESC_H
