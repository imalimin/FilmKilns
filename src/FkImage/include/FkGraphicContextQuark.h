/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICCONTEXTQUARK_H
#define FK_GRAPHIC_FKGRAPHICCONTEXTQUARK_H

#include "FkQuark.h"
#include "FkGraphicCtxComponent.h"
#include "FkRenderEngine.h"

FK_CLASS FkGraphicContextQuark FK_EXTEND FkQuark {
public:
    FkGraphicContextQuark();

    FkGraphicContextQuark(const FkGraphicContextQuark &o) = delete;

    virtual ~FkGraphicContextQuark();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

private:
    FkResult _onMakeCurrent(std::shared_ptr<FkProtocol> p);

    FkResult _onSetSurface(std::shared_ptr<FkProtocol> p);

    FkResult _onRenderRequest(std::shared_ptr<FkProtocol> p);

private:
    static const std::string FK_DETACH_CONTEXT;
    static const std::string FK_ATTACH_CONTEXT;
    static const FkID FK_MSG_NEW_LAYER;
    std::shared_ptr<FkGraphicContext> context = nullptr;
    std::shared_ptr<FkGraphicContext> contextOfWin = nullptr;
    std::shared_ptr<FkRenderEngine> engine = nullptr;
    std::shared_ptr<FkMaterialCompo> material = nullptr;
    std::shared_ptr<FkGraphicWindow> win = nullptr;
};


#endif //FK_GRAPHIC_FKGRAPHICCONTEXTQUARK_H
