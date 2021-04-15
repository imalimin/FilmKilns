/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkSizeComponent.h"

FkSizeComponent::FkSizeComponent() : FkGraphicComponent() {
    FK_MARK_SUPER

}

FkSizeComponent::FkSizeComponent(const FkSizeComponent &o) : FkGraphicComponent(o), size(o.size) {
    FK_MARK_SUPER

}

FkSizeComponent::~FkSizeComponent() {

}