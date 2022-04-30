/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkScaleTypeComponent.h"

FK_IMPL_CLASS_TYPE(FkScaleTypeComponent, FkGraphicComponent)

FkScaleTypeComponent::FkScaleTypeComponent()
        : FkGraphicComponent(), value(kScaleType::CENTER_MATRIX) {

}

FkScaleTypeComponent::FkScaleTypeComponent(const FkScaleTypeComponent &o) : FkGraphicComponent(o),
                                                                            value(o.value) {

}

FkScaleTypeComponent::~FkScaleTypeComponent() {

}