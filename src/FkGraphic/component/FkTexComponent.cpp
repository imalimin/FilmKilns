/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkTexComponent.h"

FkTexComponent::FkTexComponent() : FkBufferComponent() {
    FK_MARK_SUPER
}

FkTexComponent::FkTexComponent(const FkTexComponent &o) : FkBufferComponent(o), id(o.id) {
    FK_MARK_SUPER
}

FkTexComponent::~FkTexComponent() {
    id = FK_ID_NONE;
}
