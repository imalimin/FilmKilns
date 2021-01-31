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

FK_CLASS FkGraphicFrameObject FK_EXTEND FkSource {
public:
    FkGraphicFrameObject();

    FkGraphicFrameObject(const FkGraphicFrameObject &o) = delete;

    virtual ~FkGraphicFrameObject();

    virtual FkResult create() override;

    virtual void destroy() override;

    virtual size_t size() override;

    virtual void bind();

    virtual void unbind();

    virtual FkResult attach(std::shared_ptr<FkGraphicTexture> o);

    virtual FkResult dettach(uint32_t target);

private:
    uint32_t fbo = 0;
};

FK_CLASS FkGraphicFBOAllocator FK_EXTEND FkSourceAllocator<FkGraphicFrameObject, int32_t> {
public:
    FkGraphicFBOAllocator();

    FkGraphicFBOAllocator(const FkGraphicTexture &o) = delete;

    ~FkGraphicFBOAllocator();

    virtual FkGraphicFrameObject *delegateAlloc(int32_t &desc) override;

    virtual bool delegateEquals(int32_t &desc, FkGraphicFrameObject *fbo) override;

};


#endif //FK_GRAPHIC_FKGRAPHICFRAMEOBJECT_H
