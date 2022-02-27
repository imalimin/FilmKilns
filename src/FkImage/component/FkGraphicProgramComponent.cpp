/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicProgramComponent.h"

FkGraphicProgramComponent::FkGraphicProgramComponent() : FkGraphicComponent() {
    FK_MARK_SUPER
}

FkGraphicProgramComponent::FkGraphicProgramComponent(const FkGraphicProgramComponent &o)
        : FkGraphicComponent(), program(o.program) {
    FK_MARK_SUPER
}

FkGraphicProgramComponent::~FkGraphicProgramComponent() {
    program = nullptr;
}