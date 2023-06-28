/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkRenderRequestPrt.h"

FK_IMPL_CLASS_TYPE(FkRenderRequestPrt, FkWinSizeProto)

FkRenderRequestPrt::FkRenderRequestPrt() : FkWinSizeProto() {

}

FkRenderRequestPrt::FkRenderRequestPrt(const FkRenderRequestPrt &o) : FkWinSizeProto(o), req(o.req) {

}

FkRenderRequestPrt::~FkRenderRequestPrt() {
    req = nullptr;
}