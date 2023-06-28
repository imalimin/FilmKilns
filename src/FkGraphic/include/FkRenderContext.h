/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2022-06-11 11:07:20
*/

#ifndef FK_GRAPHIC_FKRENDERCONTEXT_H
#define FK_GRAPHIC_FKRENDERCONTEXT_H

#include "FkEngineContext.h"
#include "FkRenderEngineSettings.h"

FK_SUPER_CLASS(FkRenderContext, FkEngineContext) {
FK_DEF_CLASS_TYPE_FUNC(FkRenderContext)

public:
    static std::shared_ptr<FkRenderContext> wrap(std::shared_ptr<FkEngineContext> _context);

    FkRenderContext(const FkRenderContext &o) = delete;

    virtual ~FkRenderContext();

    virtual std::shared_ptr<FkEngineSettings> getEngineSettings() override;

    std::shared_ptr<FkRenderEngineSettings> getRenderSettings();

private:
    FkRenderContext(std::shared_ptr<FkEngineContext> &_context);

private:
    std::shared_ptr<FkEngineContext> _context = nullptr;
};

#endif //FK_GRAPHIC_FKRENDERCONTEXT_H