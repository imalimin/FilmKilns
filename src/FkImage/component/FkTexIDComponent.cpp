/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkTexIDComponent.h"

FkTexIDComponent::FkTexIDComponent() : FkBufferComponent() {
    FK_MARK_SUPER
}

FkTexIDComponent::FkTexIDComponent(const FkTexIDComponent &o) : FkBufferComponent(o), id(o.id) {
    FK_MARK_SUPER
}

FkTexIDComponent::~FkTexIDComponent() {
    id = FK_ID_NONE;
}
