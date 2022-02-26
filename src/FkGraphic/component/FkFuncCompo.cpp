/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-26 22:05:26
*/

#include "FkFuncCompo.h"

FkFuncCompo::FkFuncCompo(std::function<void()> func) : FkComponent(), func(func) {
    FK_MARK_SUPER
}

FkFuncCompo::FkFuncCompo(const FkFuncCompo &o) : FkComponent(o), func(o.func) {
    FK_MARK_SUPER
}

FkFuncCompo::~FkFuncCompo() {
    func = nullptr;
}