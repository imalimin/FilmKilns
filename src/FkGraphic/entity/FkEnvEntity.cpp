/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-20 23:29:28
*/

#include "FkEnvEntity.h"

FkEnvEntity::FkEnvEntity() : FkEntity() {
    FK_MARK_SUPER
}

FkEnvEntity::FkEnvEntity(const FkEnvEntity &o) : FkEntity(o) {
    FK_MARK_SUPER
}

FkEnvEntity::~FkEnvEntity() {

}

std::shared_ptr<FkContextCompo> FkEnvEntity::getContext() {
    return findComponent<FkContextCompo>();
}
