/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkVertexObjectComponent.h"
#include "FkGLDefinition.h"

FkVertexObjectComponent::FkVertexObjectComponent() : FkGraphicComponent() {
    FK_MARK_SUPER
}

FkVertexObjectComponent::FkVertexObjectComponent(const FkVertexObjectComponent &o)
        : FkGraphicComponent(o), vbo(o.vbo) {
    FK_MARK_SUPER
}

FkVertexObjectComponent::~FkVertexObjectComponent() {

}

void FkVertexObjectComponent::bind() {
    FkAssert(nullptr != vbo, );
    vbo->bind();
}

void FkVertexObjectComponent::unbind() {
    FkAssert(nullptr != vbo, );
    vbo->unbind();
}

FkResult FkVertexObjectComponent::setup(std::shared_ptr<FkVertexObject> _vbo,
                                        float *position, FkVertexDesc &posDesc,
                                        float *coord, FkVertexDesc &coordDesc) {
    FkAssert(nullptr != _vbo, FK_FAIL);
    this->vbo = _vbo;
    int32_t offset = 0;
    bind();
    auto size = posDesc.countVertex * posDesc.countPerVertex * posDesc.format;
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, position);
    loc.insert(std::make_pair(kValueLoc::VERTEX, std::make_pair(0, posDesc)));
    offset += size;

    size = coordDesc.countVertex * coordDesc.countPerVertex * coordDesc.format;
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, coord);
    loc.insert(std::make_pair(kValueLoc::COORDINATE, std::make_pair(offset, coordDesc)));
    unbind();
    return FK_OK;
}

FkResult FkVertexObjectComponent::getValueLoc(kValueLoc _loc, int32_t &start, FkVertexDesc &desc) {
    auto itr = loc.find(_loc);
    if (loc.end() != itr) {
        start = itr->second.first;
        desc = itr->second.second;
        return FK_OK;
    }
    return FK_FAIL;
}