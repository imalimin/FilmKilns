/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkMatrixComponent.h"

FkMatrixComponent::FkMatrixComponent() : FkGraphicComponent() {
    FK_MARK_SUPER
}

FkMatrixComponent::FkMatrixComponent(const FkMatrixComponent &o)
        : FkGraphicComponent(o), value(o.value) {
    FK_MARK_SUPER
}

FkMatrixComponent::~FkMatrixComponent() {

}