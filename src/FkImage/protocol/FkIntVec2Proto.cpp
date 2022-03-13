/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-13 13:38:23
*/

#include "FkIntVec2Proto.h"

FkIntVec2Proto::FkIntVec2Proto() : FkProtocol(),
                                   value(0, 0) {
    FK_MARK_SUPER
}

FkIntVec2Proto::FkIntVec2Proto(const FkIntVec2Proto &o)
        : FkProtocol(o),
          value(o.value) {
    FK_MARK_SUPER
}

FkIntVec2Proto::~FkIntVec2Proto() {

}