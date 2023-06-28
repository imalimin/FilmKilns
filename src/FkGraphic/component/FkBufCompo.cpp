/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-26 11:52:32
*/

#include "FkBufCompo.h"

FK_IMPL_CLASS_TYPE(FkBufCompo, FkComponent)

FkBufCompo::FkBufCompo(std::shared_ptr<FkBuffer> &buf) : FkComponent(), buf(buf) {

}

FkBufCompo::FkBufCompo(const FkBufCompo &o) : FkComponent(o) {

}

FkBufCompo::~FkBufCompo() {

}