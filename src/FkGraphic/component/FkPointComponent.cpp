/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkPointComponent.h"

FK_IMPL_CLASS_TYPE(FkPointComponent, FkComponent)

FkPointComponent::FkPointComponent()
        : FkGraphicComponent(), value(0, 0), color(FkColor::black()) {

}

FkPointComponent::FkPointComponent(const FkPointComponent &o)
        : FkGraphicComponent(o), value(o.value), color(o.color) {

}

FkPointComponent::~FkPointComponent() {

}