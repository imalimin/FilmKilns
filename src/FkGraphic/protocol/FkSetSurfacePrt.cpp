/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkSetSurfacePrt.h"


FkSetSurfacePrt::FkSetSurfacePrt() : FkProtocol() {
    FK_MARK_SUPER
}

FkSetSurfacePrt::FkSetSurfacePrt(const FkSetSurfacePrt &o) : FkProtocol(o), win(o.win) {
    FK_MARK_SUPER

}

FkSetSurfacePrt::~FkSetSurfacePrt() {
    win = nullptr;
}