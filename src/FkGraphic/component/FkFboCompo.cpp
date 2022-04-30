/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-26 20:35:11
*/

#include "FkFboCompo.h"

FK_IMPL_CLASS_TYPE(FkFboCompo, FkComponent)

FkFboCompo::FkFboCompo(std::shared_ptr<FkGraphicFrameObject> &fbo) : FkComponent(), fbo(fbo) {

}

FkFboCompo::FkFboCompo(const FkFboCompo &o) : FkComponent(o), fbo(o.fbo) {

}

FkFboCompo::~FkFboCompo() {

}