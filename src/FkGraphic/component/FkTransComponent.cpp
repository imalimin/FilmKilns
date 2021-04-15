/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkTransComponent.h"

FkTransComponent::FkTransComponent() : FkGraphicComponent(), position(0, 0) {
    FK_MARK_SUPER

}

FkTransComponent::FkTransComponent(const FkTransComponent &o)
        : FkGraphicComponent(o), position(o.position) {
    FK_MARK_SUPER

}

FkTransComponent::~FkTransComponent() {

}
