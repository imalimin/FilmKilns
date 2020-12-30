/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkHelloProt.h"

FkHelloProt::FkHelloProt() : FkProtocol(0x1) {
    FK_MARK_SUPER
}

FkHelloProt::FkHelloProt(const FkHelloProt &o) : FkProtocol(o) {

}

FkHelloProt::~FkHelloProt() {

}