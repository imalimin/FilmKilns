/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlOperateScale.h"

AlOperateScale::AlOperateScale(int32_t layerId, AlRational scale, AlVec2 anchor)
        : AlOperateDesc(layerId), scale(scale), anchor(anchor) {

}

AlOperateScale::AlOperateScale(const AlOperateScale &o)
        : AlOperateDesc(o.layerId), scale(o.scale), anchor(o.anchor) {

}

AlOperateScale::~AlOperateScale() {

}
