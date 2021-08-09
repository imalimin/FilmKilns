/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkVertexObjectComponent.h"

FkVertexObjectComponent::FkVertexObjectComponent() : FkGraphicComponent() {
    FK_MARK_SUPER
}

FkVertexObjectComponent::FkVertexObjectComponent(const FkVertexObjectComponent &o) : FkGraphicComponent(o), vbo(o.vbo) {
    FK_MARK_SUPER
}

FkVertexObjectComponent::~FkVertexObjectComponent() {

}