/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: ${author}
* CREATE TIME: 2022-2-25 13:14:50
*/

#include "FkNewTexProto.h"

FkNewTexProto::FkNewTexProto() : FkProtocol() {
    FK_MARK_SUPER
}

FkNewTexProto::FkNewTexProto(const FkNewTexProto &o) : FkProtocol(o), texEntity(o.texEntity) {
    FK_MARK_SUPER
}

FkNewTexProto::~FkNewTexProto() {

}