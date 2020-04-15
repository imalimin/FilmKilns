/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlOperateCrop.h"

AlOperateCrop::AlOperateCrop(int32_t layerId, float left, float top, float right, float bottom)
        : AlOperateDesc(layerId), rectF(left, top, right, bottom) {

}

AlOperateCrop::AlOperateCrop(const AlOperateCrop &o)
        : AlOperateDesc(layerId), rectF(o.rectF) {

}

AlOperateCrop::~AlOperateCrop() {

}
