/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALOPERATEROTATE_H
#define HWVC_ANDROID_ALOPERATEROTATE_H

#include "AlOperateDesc.h"
#include "AlRational.h"
#include "AlVec2.h"

al_class AlOperateRotate al_extend AlOperateDesc {
public:
    AlOperateRotate(int32_t layerId, AlRational rotation, AlVec2 anchor);

    AlOperateRotate(const AlOperateRotate &o);

    virtual ~AlOperateRotate();

public:
    AlRational rotation;
    AlVec2 anchor;
};


#endif //HWVC_ANDROID_ALOPERATEROTATE_H
