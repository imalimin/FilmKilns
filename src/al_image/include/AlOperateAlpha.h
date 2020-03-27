/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALOPERATEALPHA_H
#define HWVC_ANDROID_ALOPERATEALPHA_H

#include "AlOperateDesc.h"

al_class_ex(AlOperateAlpha, AlOperateDesc) {
public:
    AlOperateAlpha(int32_t layerId, float alpha);

    AlOperateAlpha(const AlOperateAlpha &o);

    virtual ~AlOperateAlpha();

public:
    float alpha;
};


#endif //HWVC_ANDROID_ALOPERATEALPHA_H
