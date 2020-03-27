/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlOperateAlpha.h"

AlOperateAlpha::AlOperateAlpha(int32_t layerId, float alpha)
        : AlOperateDesc(layerId), alpha(alpha) {

}

AlOperateAlpha::AlOperateAlpha(const AlOperateAlpha &o)
        : AlOperateDesc(o.layerId), alpha(o.alpha) {

}

AlOperateAlpha::~AlOperateAlpha() {

}
