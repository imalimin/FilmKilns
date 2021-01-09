/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkCalculateProt.h"

FkCalculateProt::FkCalculateProt() : FkProtocol() {
    FK_MARK_SUPER
}

FkCalculateProt::FkCalculateProt(const FkCalculateProt &o) : FkProtocol(o), number(o.number) {

}

FkCalculateProt::~FkCalculateProt() {

}