/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-04-02 22:56:32
*/

#include "FkTexFuncCompo.h"

FK_IMPL_CLASS_TYPE(FkTexFuncCompo, FkFuncCompo)

FkTexFuncCompo::FkTexFuncCompo(std::function<void(uint32_t, FkSize, int64_t)> func)
        : FkFuncCompo(nullptr), texFunc(func) {

}

FkTexFuncCompo::FkTexFuncCompo(const FkTexFuncCompo &o) : FkFuncCompo(o), texFunc(o.texFunc) {

}

FkTexFuncCompo::~FkTexFuncCompo() {
    texFunc = nullptr;
}