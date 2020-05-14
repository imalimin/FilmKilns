/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALOPERATEDESC_H
#define HWVC_ANDROID_ALOPERATEDESC_H

#include "Object.h"

al_class AlOperateDesc al_extend Object {
public:
    enum class CoordIdx : int {
        UNKNOWN = 0,
        WINDOW,
        CANVAS,
        LAYER
    };
public:
    AlOperateDesc(int32_t layerId);

    AlOperateDesc(const AlOperateDesc &o);

    virtual ~AlOperateDesc();

public:
    int32_t layerId;
    CoordIdx coordIdx = CoordIdx::WINDOW;
};


#endif //HWVC_ANDROID_ALOPERATEDESC_H
