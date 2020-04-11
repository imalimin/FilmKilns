/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALOPERATECROP_H
#define HWVC_ANDROID_ALOPERATECROP_H

#include "AlOperateDesc.h"
#include "AlRectF.h"

al_class_ex(AlOperateCrop, AlOperateDesc) {
public:
    AlOperateCrop(int32_t layerId, float left, float top, float right, float bottom);

    AlOperateCrop(const AlOperateCrop &o);

    virtual ~AlOperateCrop();

public:
    AlRectF rectF;
    bool align16 = false;
};


#endif //HWVC_ANDROID_ALOPERATECROP_H
