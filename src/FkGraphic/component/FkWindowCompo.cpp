/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-28 22:22:17
*/

#include "FkWindowCompo.h"

FK_IMPL_CLASS_TYPE(FkWindowCompo, FkComponent)

FkWindowCompo::FkWindowCompo(std::shared_ptr<FkGraphicWindow> &_win) : FkComponent(), win(_win) {

}

FkWindowCompo::FkWindowCompo(const FkWindowCompo &o) : FkComponent(o), win(o.win) {

}

FkWindowCompo::~FkWindowCompo() {

}