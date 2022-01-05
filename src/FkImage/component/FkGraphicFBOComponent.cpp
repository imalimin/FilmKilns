/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicFBOComponent.h"

FkGraphicFBOComponent::FkGraphicFBOComponent() : FkGraphicComponent() {
    FK_MARK_SUPER
}

FkGraphicFBOComponent::FkGraphicFBOComponent(const FkGraphicFBOComponent &o)
        : FkGraphicComponent(), fbo(o.fbo) {
    FK_MARK_SUPER
}

FkGraphicFBOComponent::~FkGraphicFBOComponent() {
    fbo = nullptr;
}
