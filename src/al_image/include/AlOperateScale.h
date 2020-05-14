/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALOPERATESCALE_H
#define HWVC_ANDROID_ALOPERATESCALE_H

#include "AlOperateDesc.h"
#include "AlRational.h"
#include "AlVec2.h"

al_class AlOperateScale al_extend AlOperateDesc {
public:
    AlOperateScale(int32_t layerId, AlRational scale, AlVec2 anchor);

    AlOperateScale(const AlOperateScale &o);

    virtual ~AlOperateScale();

public:
    AlRational scaleX;
    AlRational scaleY;
    AlVec2 anchor;
};


#endif //HWVC_ANDROID_ALOPERATESCALE_H
