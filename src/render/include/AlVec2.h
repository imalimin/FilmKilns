/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALVEC2_H
#define HWVC_ANDROID_ALVEC2_H

#include "Object.h"
/**
 * position or scale params
 */
al_class(AlVec2) {
public:
    AlVec2(float x = 0.0f, float y = 0.0f);

    AlVec2(const AlVec2 &o);

    ~AlVec2();

public:
    float x = 0.0f;
    float y = 0.0f;
};


#endif //HWVC_ANDROID_ALVEC2_H
