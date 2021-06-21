/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkRenderRequestPrt.h"

FkRenderRequestPrt::FkRenderRequestPrt() : FkProtocol() {
    FK_MARK_SUPER
}

FkRenderRequestPrt::FkRenderRequestPrt(const FkRenderRequestPrt &o) : FkProtocol(o), req(o.req) {
    FK_MARK_SUPER
}

FkRenderRequestPrt::~FkRenderRequestPrt() {
    req = nullptr;
}