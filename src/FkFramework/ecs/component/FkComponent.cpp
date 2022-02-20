/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-20 16:57:35
*/

#include "FkComponent.h"

FkComponent::FkComponent() : FkObject() {
    FK_MARK_SUPER
}

FkComponent::FkComponent(const FkComponent &o) : FkObject(o) {
    FK_MARK_SUPER
}

FkComponent::~FkComponent() {

}