/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-4-19 23:37:24
*/

#include "FkReadPixelsProto.h"

FkReadPixelsProto::FkReadPixelsProto() : FkLayerProto(), size(0) {
    FK_MARK_SUPER
}

FkReadPixelsProto::FkReadPixelsProto(const FkReadPixelsProto &o)
        : FkLayerProto(o), buf(o.buf), size(o.size) {
    FK_MARK_SUPER
}

FkReadPixelsProto::~FkReadPixelsProto() {

}