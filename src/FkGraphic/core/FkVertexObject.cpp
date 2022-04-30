/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkVertexObject.h"
#include "FkGLDefinition.h"

FK_IMPL_CLASS_TYPE(FkVertexDesc, FkObject)
FK_IMPL_CLASS_TYPE(FkVBODescription, FkObject)
FK_IMPL_CLASS_TYPE(FkVertexObject, FkSource)
FK_IMPL_CLASS_TYPE(FkVertexObjectAllocator, FkSourceAllocator)

#define TAG "FkVBODescription"

FkVertexDesc::FkVertexDesc() : FkObject() {

}

FkVertexDesc::FkVertexDesc(const FkVertexDesc &o) : FkObject(),
                                                    countVertex(o.countVertex),
                                                    countPerVertex(o.countPerVertex),
                                                    format(o.format) {

}

FkVertexDesc::~FkVertexDesc() {

}

FkVBODescription::FkVBODescription(size_t size) : FkObject(), size(size) {

}

FkVBODescription::FkVBODescription(const FkVBODescription &o) : FkObject(), size(o.size) {

}

FkVBODescription::~FkVBODescription() {

}

FkVertexObject::FkVertexObject(FkVBODescription &desc) : FkSource(), desc(desc) {

}

FkVertexObject::~FkVertexObject() {
    FkAssert(GL_NONE == vbo,);
}

FkResult FkVertexObject::create() {
    if (GL_NONE == vbo) {
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, desc.size, nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
        return FK_OK;
    }
    return FK_FAIL;
}

void FkVertexObject::destroy() {
    if (GL_NONE != vbo) {
        glDeleteBuffers(1, &vbo);
        vbo = GL_NONE;
    }
}

size_t FkVertexObject::size() {
    return desc.size;
}

void FkVertexObject::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void FkVertexObject::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
}


FkResult FkVertexObject::set(float *data, size_t size, int32_t offset) {
    return FK_OK;
}

FkVertexObjectAllocator::FkVertexObjectAllocator(int capacity)
        : FkSourceAllocator<FkVertexObject, FkVBODescription>(capacity) {

}

FkVertexObjectAllocator::~FkVertexObjectAllocator() {

}

FkVertexObject *FkVertexObjectAllocator::delegateAlloc(FkVBODescription &desc) {
    FkAssert(EGL_NO_CONTEXT != eglGetCurrentContext(), nullptr);
    if (0 == desc.size) {
        FkLogE(TAG, "Allocate texture failed. Invalid size(%d)", desc.size);
    }
    auto o = new FkVertexObject(desc);
    if (FK_OK != o->create()) {
        delete o;
        return nullptr;
    }
    return o;
}

bool FkVertexObjectAllocator::delegateEquals(FkVBODescription &desc, FkVertexObject *tex) {
    return desc.size == tex->size();
}
