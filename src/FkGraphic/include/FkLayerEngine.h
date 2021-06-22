/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKIMAGEENGINE_H
#define FK_GRAPHIC_FKIMAGEENGINE_H

#include "FkEngine.h"
#include "FkGraphicMolecule.h"
#include "FkColor.h"
#include "FkSize.h"
#include "FkGraphicWindow.h"

FK_CLASS FkLayerEngine FK_EXTEND FkEngine {
public:
    FkLayerEngine(std::string name);

    FkLayerEngine(const FkLayerEngine &o) = delete;

    virtual ~FkLayerEngine();

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

    FkResult notifyRender();

    FkResult setSurface(std::shared_ptr<FkGraphicWindow> win);

    FkID newLayer();

    FkID newLayerWithColor(FkSize size, FkColor color);

protected:
    std::shared_ptr<FkSessionClient> getClient() { return client; };

    std::shared_ptr<FkGraphicMolecule> getMolecule() { return molecule; };

private:
    FkResult _newLayer(std::shared_ptr<FkMessage> msg);

    FkResult _updateLayerWithColor(std::shared_ptr<FkMessage> msg);

    FkResult _setSurface(std::shared_ptr<FkMessage> msg);

    FkResult _notifyRender(std::shared_ptr<FkMessage> msg);

private:
    static const FkID FK_MSG_NEW_LAYER;
    static const FkID FK_MSG_UPDATE_LAYER_WITH_COLOR;
    static const FkID FK_MSG_SET_SURFACE;
    static const FkID FK_MSG_NOTIFY_RENDER;
    std::shared_ptr<FkSessionClient> client;
    std::shared_ptr<FkGraphicMolecule> molecule;
};


#endif //FK_GRAPHIC_FKIMAGEENGINE_H
