/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlOperateTrans.h"

AlOperateTrans::AlOperateTrans(int32_t layerId, float x, float y)
        : AlOperateDesc(layerId), x(x), y(y) {

}

AlOperateTrans::AlOperateTrans(const AlOperateTrans &o)
        : AlOperateDesc(o.layerId), x(o.x), y(o.y) {

}

AlOperateTrans::~AlOperateTrans() {

}
