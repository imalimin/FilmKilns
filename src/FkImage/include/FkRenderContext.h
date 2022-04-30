/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-2 21:20:27
*/

#ifndef FK_IMAGE_FKRENDERCONTEXT_H
#define FK_IMAGE_FKRENDERCONTEXT_H

#include "FkEntity.h"
#include "FkQuarkContext.h"
#include "FkRenderEngine.h"

FK_SUPER_CLASS(FkRenderContext, FkEntity) {
FK_DEF_CLASS_TYPE_FUNC(FkRenderContext)

public:
    static std::shared_ptr<FkRenderContext> wrap(std::shared_ptr<FkQuarkContext> context);

public:
    virtual ~FkRenderContext();

    std::shared_ptr<FkRenderEngine> getRenderEngine();

private:
    FkRenderContext(std::shared_ptr<FkQuarkContext> &_context);

    FkRenderContext(const FkRenderContext &o) = delete;

private:
    std::shared_ptr<FkQuarkContext> context = nullptr;
};

#endif //FK_IMAGE_FKRENDERCONTEXT_H