/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicCtxComponent.h"

FkGraphicCtxComponent::FkGraphicCtxComponent() : FkGraphicComponent() {
    FK_MARK_SUPER
}

FkGraphicCtxComponent::FkGraphicCtxComponent(const FkGraphicCtxComponent &o)
        : FkGraphicComponent(o) {
    FK_MARK_SUPER
}

FkGraphicCtxComponent::~FkGraphicCtxComponent() {
    context = nullptr;
}