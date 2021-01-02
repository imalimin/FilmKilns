/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkProtocol.h"

FkProtocol::FkProtocol() : FkObject() {
    FK_MARK_SUPER
}

FkProtocol::FkProtocol(const FkProtocol &o) : FkObject() {

}

FkProtocol::~FkProtocol() {

}

FkProtocol::Type FkProtocol::getType() {
    return getClassType()->hashCode();
}
