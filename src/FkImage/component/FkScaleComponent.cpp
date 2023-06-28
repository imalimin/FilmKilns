/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkScaleComponent.h"

FK_IMPL_CLASS_TYPE(FkScaleComponent, FkGraphicComponent)

FkScaleComponent::FkScaleComponent() : FkGraphicComponent(), value(1.0f, 1.0f, 1.0f) {

}

FkScaleComponent::FkScaleComponent(FkFloatVec3 value) : FkGraphicComponent(), value(value) {

}

FkScaleComponent::FkScaleComponent(const FkScaleComponent &o)
        : FkGraphicComponent(o), value(o.value) {

}

FkScaleComponent::~FkScaleComponent() {

}
