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

    FkResult setSurface(std::shared_ptr<FkGraphicWindow> win);

    FkID newLayer();

    FkID newLayerWithFile(std::string path);

    FkID newLayerWithColor(FkSize size, FkColor color);

private:
    FkResult _newLayer(std::shared_ptr<FkMessage> msg);

    FkResult _updateLayerWithFile(std::shared_ptr<FkMessage> msg);

    FkResult _updateLayerWithColor(std::shared_ptr<FkMessage> msg);

private:
    static const FkID FK_MSG_NEW_LAYER;
    static const FkID FK_MSG_UPDATE_LAYER_WITH_FILE;
    static const FkID FK_MSG_UPDATE_LAYER_WITH_COLOR;
    std::shared_ptr<FkSessionClient> client;
    std::shared_ptr<FkGraphicMolecule> molecule;
};


#endif //FK_GRAPHIC_FKIMAGEENGINE_H
