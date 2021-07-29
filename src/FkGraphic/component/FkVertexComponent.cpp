/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkVertexComponent.h"

FkVertexComponent::FkVertexComponent() : FkGraphicComponent() {
    FK_MARK_SUPER
}

FkVertexComponent::FkVertexComponent(const FkVertexComponent &o)
        : FkGraphicComponent(o), countVertex(o.countVertex), countPerVertex(o.countPerVertex),
          offset(o.offset), byteOfData(o.byteOfData) {
    FK_MARK_SUPER
}

FkVertexComponent::~FkVertexComponent() {

}
