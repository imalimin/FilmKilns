/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALPOINTF_H
#define HWVC_ANDROID_ALPOINTF_H

#include "Object.h"
#include "AlVec2.h"

al_class_ex(AlPointF, AlVec2) {
public:
    AlPointF(float x = 0.0f, float y = 0.0f);

    AlPointF(const AlVec2 &o);

    virtual ~AlPointF();

};


#endif //HWVC_ANDROID_ALPOINTF_H
