/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkPointComponent.h"

FkPointComponent::FkPointComponent()
        : FkGraphicComponent(), value(0, 0), color(FkColor::black()) {
    FK_MARK_SUPER
}

FkPointComponent::FkPointComponent(const FkPointComponent &o)
        : FkGraphicComponent(o), value(o.value), color(o.color) {
    FK_MARK_SUPER
}

FkPointComponent::~FkPointComponent() {

}