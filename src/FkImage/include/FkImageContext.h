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

#include "FkEngineContext.h"
#include "FkRenderEngine.h"

FK_SUPER_CLASS(FkImageContext, FkEngineContext) {
FK_DEF_CLASS_TYPE_FUNC(FkImageContext)

public:
    static std::shared_ptr<FkImageContext> wrap(std::shared_ptr<FkEngineContext> _context);

    FkImageContext(const FkImageContext &o) = delete;

    virtual ~FkImageContext();

    virtual std::shared_ptr<FkEngineSettings> getEngineSettings() override;

    std::shared_ptr<FkRenderEngine> getRenderEngine();

private:
    FkImageContext(std::shared_ptr<FkEngineContext> &_context);

private:
    std::shared_ptr<FkEngineContext> _context = nullptr;
};

#endif //FK_IMAGE_FKRENDERCONTEXT_H