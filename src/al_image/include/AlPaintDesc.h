/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALPAINTDESC_H
#define HWVC_ANDROID_ALPAINTDESC_H

#include "Object.h"
#include "AlVec2.h"

al_class(AlPaintDesc) {
public:
    AlPaintDesc(int32_t layerId, float x, float y, bool painting);

    AlPaintDesc(const AlPaintDesc &o);

    virtual ~AlPaintDesc();

public:
    int32_t layerId;
    bool painting;
    AlVec2 point;
};


#endif //HWVC_ANDROID_ALPAINTDESC_H
