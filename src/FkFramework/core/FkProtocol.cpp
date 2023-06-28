/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkProtocol.h"

FK_IMPL_CLASS_TYPE(FkProtocol, FkObject)

FkProtocol::FkProtocol() : FkObject() {

}

FkProtocol::FkProtocol(const FkProtocol &o) : FkObject() {


}

FkProtocol::~FkProtocol() {

}

FkProtocol::Type FkProtocol::getType() {
    return getClassType().getId();
}
