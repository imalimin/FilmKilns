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

FK_CLASS FkLayerEngine FK_EXTEND FkEngine {
public:
    FkLayerEngine(std::string name);

    FkLayerEngine(const FkLayerEngine &o) = delete;

    virtual ~FkLayerEngine();

    FkResult create();

    FkResult destroy();

    FkResult start();

    FkResult stop();

    FkID newLayer(std::string path);

private:

    FkResult _newLayer(std::shared_ptr<FkMessage> msg);

private:
    static const FkID FK_MSG_NEW_LAYER;
    std::shared_ptr<FkSessionClient> client;
    std::shared_ptr<FkGraphicMolecule> molecule;
};


#endif //FK_GRAPHIC_FKIMAGEENGINE_H
