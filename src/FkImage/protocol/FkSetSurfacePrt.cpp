/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkSetSurfacePrt.h"

FK_IMPL_CLASS_TYPE(FkSetSurfacePrt, FkProtocol)

FkSetSurfacePrt::FkSetSurfacePrt() : FkProtocol() {

}

FkSetSurfacePrt::FkSetSurfacePrt(const FkSetSurfacePrt &o) : FkProtocol(o), win(o.win) {


}

FkSetSurfacePrt::~FkSetSurfacePrt() {
    win = nullptr;
}