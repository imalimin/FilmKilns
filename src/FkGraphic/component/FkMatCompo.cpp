/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-27 16:26:01
*/

#include "FkMatCompo.h"

FkMatCompo::FkMatCompo() : FkComponent() {
    FK_MARK_SUPER
}

FkMatCompo::FkMatCompo(const FkMatCompo &o) : FkComponent(o), value(o.value) {
    FK_MARK_SUPER
}

FkMatCompo::~FkMatCompo() {

}