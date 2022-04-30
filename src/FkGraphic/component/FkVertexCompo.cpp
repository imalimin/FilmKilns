/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-27 14:40:42
*/

#include "FkVertexCompo.h"

FK_IMPL_CLASS_TYPE(FkVertexCompo, FkComponent)

FkVertexCompo::FkVertexCompo() : FkVertexCompo(nullptr) {
}

FkVertexCompo::FkVertexCompo(std::shared_ptr<FkBuffer> buf) : FkBufCompo(buf), desc() {

}

FkVertexCompo::FkVertexCompo(const FkVertexCompo &o) : FkBufCompo(o), desc(o.desc) {

    if (nullptr != o.data()) {
        auto size = o.desc.countVertex * o.desc.countPerVertex * o.desc.format;
        memcpy(data(), o.data(), size);
    }
}

FkVertexCompo::~FkVertexCompo() {
    buf = nullptr;
}

FkResult FkVertexCompo::setup(size_t _countVertex, size_t _countPerVertex,
                              size_t _bytes, void *_data) {
    desc.countVertex = _countVertex;
    desc.countPerVertex = _countPerVertex;
    desc.format = _bytes;
    FkAssert(size() > 0, FK_FAIL);

    buf = FkBuffer::alloc(size());
    memcpy(data(), _data, size());
    return FK_OK;
}

void *FkVertexCompo::data() const {
    return buf ? buf->data() : nullptr;
}

size_t FkVertexCompo::size() {
    return desc.countVertex * desc.countPerVertex * desc.format;
}