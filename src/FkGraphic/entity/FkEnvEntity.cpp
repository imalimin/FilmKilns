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

FK_IMPL_CLASS_TYPE(FkEnvEntity, FkEntity)

FkEnvEntity::FkEnvEntity() : FkEntity() {

}

FkEnvEntity::FkEnvEntity(const FkEnvEntity &o) : FkEntity(o) {

}

FkEnvEntity::~FkEnvEntity() {

}

std::shared_ptr<FkContextCompo> FkEnvEntity::getContext() {
    return FK_FIND_COMPO(this, FkContextCompo);
}
