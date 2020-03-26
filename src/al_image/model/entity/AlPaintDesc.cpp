/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlPaintDesc.h"

AlPaintDesc::AlPaintDesc(int32_t layerId, float x, float y, bool painting)
        : Object(), layerId(layerId), point(x, y), painting(painting) {

}

AlPaintDesc::AlPaintDesc(const AlPaintDesc &o)
        : Object(), layerId(o.layerId), point(o.point), painting(o.painting) {

}

AlPaintDesc::~AlPaintDesc() {

}
