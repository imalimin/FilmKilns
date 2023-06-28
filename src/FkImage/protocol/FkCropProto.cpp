/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-27 14:57:36
*/

#include "FkCropProto.h"

FK_IMPL_CLASS_TYPE(FkCropProto, FkProtocol)

FkCropProto::FkCropProto()
        : FkProtocol(), layerId(FK_ID_NONE),
          leftTop(0, 0), leftBottom(0, 0),
          rightTop(0, 0), rightBottom(0, 0) {

}

FkCropProto::FkCropProto(const FkCropProto &o)
        : FkProtocol(o), layerId(o.layerId), canvas(o.canvas),
          leftTop(o.leftTop), leftBottom(o.leftBottom),
          rightTop(o.rightTop), rightBottom(o.rightBottom) {

}

FkCropProto::~FkCropProto() {

}