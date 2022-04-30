/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkTransComponent.h"

FK_IMPL_CLASS_TYPE(FkTransComponent, FkGraphicComponent)

FkTransComponent::FkTransComponent() : FkGraphicComponent(), value(0, 0) {


}

FkTransComponent::FkTransComponent(const FkTransComponent &o)
        : FkGraphicComponent(o), value(o.value) {


}

FkTransComponent::~FkTransComponent() {

}
