/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-5 20:44:47
*/

#include "FkRenderEntity.h"

FkRenderEntity::FkRenderEntity() : FkEntity() {
    FK_MARK_SUPER
}

FkRenderEntity::FkRenderEntity(const FkRenderEntity &o) : FkEntity(o) {
    FK_MARK_SUPER
}

FkRenderEntity::~FkRenderEntity() {

}