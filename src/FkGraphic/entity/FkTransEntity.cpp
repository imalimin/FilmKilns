/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-20 23:30:10
*/

#include "FkTransEntity.h"
#include "FkTransComponent.h"

FkTransEntity::FkTransEntity() : FkEntity() {
    FK_MARK_SUPER
}

FkTransEntity::FkTransEntity(const FkTransEntity &o) : FkEntity(o) {
    FK_MARK_SUPER
}

FkTransEntity::~FkTransEntity() {

}

void FkTransEntity::setTranslate(int32_t x, int32_t y) {
    auto compo = std::make_shared<FkTransComponent>();
    compo->value.x = x;
    compo->value.y = y;
    addComponent(compo);
}
