/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkPositionComponent.h"

FkPositionComponent::FkPositionComponent() : FkVertexComponent() {
    FK_MARK_SUPER
}

FkPositionComponent::FkPositionComponent(const FkPositionComponent &o) : FkVertexComponent(o) {
    FK_MARK_SUPER
}

FkPositionComponent::~FkPositionComponent() {

}