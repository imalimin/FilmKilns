/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlOperateDesc.h"
#include "AlIdentityCreator.h"

AlOperateDesc::AlOperateDesc(int32_t layerId)
        : Object(), layerId(layerId), coordIdx(CoordIdx::WINDOW) {

}

AlOperateDesc::AlOperateDesc(const AlOperateDesc &o)
        : Object(), layerId(o.layerId), coordIdx(o.coordIdx) {

}

AlOperateDesc::~AlOperateDesc() {

}
