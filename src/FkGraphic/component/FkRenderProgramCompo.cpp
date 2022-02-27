/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-27 15:59:08
*/

#include "FkRenderProgramCompo.h"

FkRenderProgramCompo::FkRenderProgramCompo() : FkComponent() {
    FK_MARK_SUPER
}

FkRenderProgramCompo::FkRenderProgramCompo(const FkRenderProgramCompo &o) : FkComponent(o), program(o.program) {
    FK_MARK_SUPER
}

FkRenderProgramCompo::~FkRenderProgramCompo() {
    program = nullptr;
}