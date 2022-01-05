/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkColorComponent.h"

FkColorComponent::FkColorComponent() : FkGraphicComponent() {
    FK_MARK_SUPER
}

FkColorComponent::FkColorComponent(const FkColorComponent &o)
        : FkGraphicComponent(o), color(o.color) {
    FK_MARK_SUPER
}

FkColorComponent::~FkColorComponent() {

}