/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkRenderProto.h"

FkRenderProto::FkRenderProto() : FkProtocol() {
    FK_MARK_SUPER
}

FkRenderProto::FkRenderProto(const FkRenderProto &o) : FkProtocol(o), env(o.env),
                                                       materials(o.materials), device(o.device) {
    FK_MARK_SUPER
}

FkRenderProto::~FkRenderProto() {

}