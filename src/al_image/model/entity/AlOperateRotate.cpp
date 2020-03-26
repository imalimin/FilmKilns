/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlOperateRotate.h"

AlOperateRotate::AlOperateRotate(int32_t layerId, AlRational rotation, AlVec2 anchor)
        : AlOperateDesc(layerId), rotation(rotation), anchor(anchor) {

}

AlOperateRotate::AlOperateRotate(const AlOperateRotate &o)
        : AlOperateDesc(o.layerId), rotation(o.rotation), anchor(o.anchor){

}

AlOperateRotate::~AlOperateRotate() {

}
