/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALOPERATETRANS_H
#define HWVC_ANDROID_ALOPERATETRANS_H

#include "AlOperateDesc.h"

al_class_ex(AlOperateTrans, AlOperateDesc) {
public:
    AlOperateTrans(int32_t layerId, float x, float y);

    AlOperateTrans(const AlOperateTrans &o);

    virtual ~AlOperateTrans();

public:
    float x, y;
};


#endif //HWVC_ANDROID_ALOPERATETRANS_H
