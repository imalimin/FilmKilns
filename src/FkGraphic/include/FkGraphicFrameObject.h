/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICFRAMEOBJECT_H
#define FK_GRAPHIC_FKGRAPHICFRAMEOBJECT_H

#include "FkSource.h"
#include "FkDefinition.h"
#include "FkGraphicTexture.h"

FK_SUPER_CLASS(FkGraphicFrameObject, FkSource) {
FK_DEF_CLASS_TYPE_FUNC(FkGraphicFrameObject)

public:
    FkGraphicFrameObject();

    FkGraphicFrameObject(const FkGraphicFrameObject &o) = delete;

    virtual ~FkGraphicFrameObject();

    virtual FkResult create() override;

    virtual void destroy() override;

    virtual size_t size() override;

    virtual void bind();

    virtual void unbind();

    virtual FkResult attach(std::shared_ptr<FkGraphicTexture> &tex);

    virtual FkResult attach(std::vector<std::shared_ptr<FkGraphicTexture>> &texArray);

    virtual FkResult detach(uint32_t target);

private:
    uint32_t fbo = 0;
    std::vector<int32_t> attachments;
};

FK_SUPER_TEMPLATE_CLASS_IMPL(FkGraphicFBOAllocator, FkSourceAllocator)<FkGraphicFrameObject, int32_t> {
FK_DEF_CLASS_TYPE_FUNC(FkGraphicFBOAllocator)

public:
    FkGraphicFBOAllocator(int capacity);

    FkGraphicFBOAllocator(const FkGraphicTexture &o) = delete;

    ~FkGraphicFBOAllocator();

    virtual FkGraphicFrameObject *delegateAlloc(int32_t &desc) override;

    virtual bool delegateEquals(int32_t &desc, FkGraphicFrameObject *fbo) override;

};


#endif //FK_GRAPHIC_FKGRAPHICFRAMEOBJECT_H
