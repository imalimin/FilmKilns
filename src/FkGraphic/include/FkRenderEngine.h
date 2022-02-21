/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKRENDERENGINE_H
#define FK_GRAPHIC_FKRENDERENGINE_H

#include "FkEngine.h"
#include "FkSize.h"

FK_CLASS FkRenderEngine FK_EXTEND FkEngine {
public:
    FkRenderEngine(std::string name);

    FkRenderEngine(const FkRenderEngine &o) = delete;

    virtual ~FkRenderEngine();

    FkResult render();

    FkID newMaterial();

protected:
    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

    FkResult _onRender(std::shared_ptr<FkMessage> msg);

    FkResult _onNewMaterial(std::shared_ptr<FkMessage> msg);

private:
    static const FkID FK_MSG_RENDER;
    static const FkID FK_MSG_NEW_MATERIAL;
    std::shared_ptr<FkSessionClient> client;
    std::shared_ptr<FkQuark> molecule;
};

#endif //FK_GRAPHIC_FKRENDERENGINE_H