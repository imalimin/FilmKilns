/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKVERTEXOBJECT_H
#define FK_GRAPHIC_FKVERTEXOBJECT_H

#include "FkSource.h"

FK_SUPER_CLASS(FkVertexDesc, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkVertexDesc)

public:
    FkVertexDesc();

    FkVertexDesc(const FkVertexDesc &o);

    virtual ~FkVertexDesc();

public:
    size_t countVertex = 0;
    size_t countPerVertex = 0;
    size_t format = 0;
};

FK_SUPER_CLASS(FkVBODescription, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkVBODescription)

public:
    FkVBODescription(size_t size);

    FkVBODescription(const FkVBODescription &o);

    virtual ~FkVBODescription();

public:
    size_t size = 0;
};

FK_SUPER_CLASS(FkVertexObject, FkSource) {
FK_DEF_CLASS_TYPE_FUNC(FkVertexObject)

public:
    FkVertexObject(FkVBODescription &desc);

    FkVertexObject(const FkVertexObject &o) = delete;

    virtual ~FkVertexObject();

    virtual FkResult create() override;

    virtual void destroy() override;

    virtual size_t size() override;

    virtual void bind();

    virtual void unbind();

    virtual FkResult set(float *data, size_t size, int32_t offset);

private:
    FkVBODescription desc;
    uint32_t vbo = 0;
};

FK_SUPER_TEMPLATE_CLASS_IMPL(FkVertexObjectAllocator, FkSourceAllocator)<FkVertexObject, FkVBODescription> {
FK_DEF_CLASS_TYPE_FUNC(FkVertexObjectAllocator)

public:
    FkVertexObjectAllocator(int capacity);

    FkVertexObjectAllocator(const FkVertexObjectAllocator &o) = delete;

    virtual ~FkVertexObjectAllocator();

    virtual FkVertexObject *delegateAlloc(FkVBODescription &desc) override;

    virtual bool delegateEquals(FkVBODescription &desc, FkVertexObject *tex) override;
};

#endif //FK_GRAPHIC_FKVERTEXOBJECT_H