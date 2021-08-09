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
          byteOfData(o.byteOfData) {
    FK_MARK_SUPER
    if (nullptr != o.data) {
        memcpy(data, o.data, o.byteOfData);
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
    countVertex = _countVertex;
    countPerVertex = _countPerVertex;
    byteOfData = _countVertex * _countPerVertex * _bytes;
    FkAssert(byteOfData > 0, FK_FAIL);
    data = malloc(byteOfData);
    memcpy(data, _data, byteOfData);
    return FK_OK;
}

void *FkVertexComponent::getData() {
    return data;
}

size_t FkVertexComponent::getSize() {
    return byteOfData;
}
