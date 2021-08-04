/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkShapeComponent.h"

FkShapeComponent::FkShapeComponent() : FkGraphicComponent() {
    FK_MARK_SUPER
}

FkShapeComponent::FkShapeComponent(const FkShapeComponent &o)
        : FkGraphicComponent(o), type(o.type) {
    FK_MARK_SUPER
}

FkShapeComponent::~FkShapeComponent() {

}