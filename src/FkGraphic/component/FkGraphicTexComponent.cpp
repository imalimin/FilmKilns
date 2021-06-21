/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicTexComponent.h"

FkGraphicTexComponent::FkGraphicTexComponent() : FkGraphicComponent() {
    FK_MARK_SUPER
}

FkGraphicTexComponent::FkGraphicTexComponent(const FkGraphicTexComponent &o)
        : FkGraphicComponent(o), tex(o.tex) {
    FK_MARK_SUPER
}

FkGraphicTexComponent::~FkGraphicTexComponent() {
    tex = nullptr;
}
