/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkVertexComponent.h"

FkVertexComponent::FkVertexComponent() : FkGraphicComponent(), desc() {
    FK_MARK_SUPER
}

FkVertexComponent::FkVertexComponent(const FkVertexComponent &o)
        : FkGraphicComponent(o), desc(o.desc) {
    FK_MARK_SUPER
    if (nullptr != o.data) {
        auto size = o.desc.countVertex * o.desc.countPerVertex * o.desc.format;
        memcpy(data, o.data, size);
    }
}

FkVertexComponent::~FkVertexComponent() {
    if (nullptr != data) {
        free(data);
        data = nullptr;
    }
}

FkResult FkVertexComponent::setup(size_t _countVertex, size_t _countPerVertex,
                                  size_t _bytes, void *_data) {
    desc.countVertex = _countVertex;
    desc.countPerVertex = _countPerVertex;
    desc.format = _bytes;
    FkAssert(getSize() > 0, FK_FAIL);
    data = malloc(getSize());
    memcpy(data, _data, getSize());
    return FK_OK;
}

void *FkVertexComponent::getData() {
    return data;
}

size_t FkVertexComponent::getSize() {
    return desc.countVertex * desc.countPerVertex * desc.format;
}
