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
#include "FkRational.h"
#include "FkGraphicLayer.h"

FK_CLASS FkLayerEngine FK_EXTEND FkEngine {
public:
    FkLayerEngine(std::shared_ptr<FkEngine> &renderEngine, std::string name);

    FkLayerEngine(const FkLayerEngine &o) = delete;

    virtual ~FkLayerEngine();

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

    FkResult setSurface(std::shared_ptr<FkGraphicWindow> win);

    FkResult notifyRender();

    FkID newLayer(FkID expectId = FK_ID_NONE);

    FkID newLayerWithColor(FkSize size, FkColor color);

    FkResult setCanvasSize(FkSize size);

    FkResult postTranslate(FkID layer, int32_t dx, int32_t dy);

    FkResult setTranslate(FkID layer, int32_t x, int32_t y);

    FkResult postScale(FkID layer, float dx, float dy);

    FkResult setScale(FkID layer, float x, float y);

    FkResult postRotation(FkID layer, FkRational &rational);

    FkResult setRotation(FkID layer, FkRational &rational);

    FkResult drawPoint(FkID layer, FkColor color, int32_t size, int32_t x, int32_t y);

    FkResult queryLayers(std::vector<std::shared_ptr<FkGraphicLayer>> &vec);

    FkResult crop(FkID layer, FkIntVec2 leftTop, FkIntVec2 rightBottom);

    FkResult cropLayer(FkID layer, FkIntVec2 leftTop, FkIntVec2 rightBottom);

protected:
    std::shared_ptr<FkSessionClient> getClient() { return client; };

    std::shared_ptr<FkGraphicMolecule> getMolecule() { return molecule; };

    FkResult setCanvasSizeInternal(FkSize &size, bool isInitialize);

private:
    FkResult _setSurface(std::shared_ptr<FkMessage> msg);

    FkResult _notifyRender(std::shared_ptr<FkMessage> msg);

    FkResult _newLayer(std::shared_ptr<FkMessage> msg);

    FkResult _updateLayerWithColor(std::shared_ptr<FkMessage> msg);

    FkResult _setCanvasSize(std::shared_ptr<FkMessage> msg);

    FkResult _postTranslate(std::shared_ptr<FkMessage> msg);

    FkResult _setTranslate(std::shared_ptr<FkMessage> msg);

    FkResult _postScale(std::shared_ptr<FkMessage> msg);

    FkResult _setScale(std::shared_ptr<FkMessage> msg);

    FkResult _postRotation(std::shared_ptr<FkMessage> msg);

    FkResult _setRotation(std::shared_ptr<FkMessage> msg);

    FkResult _drawPoint(std::shared_ptr<FkMessage> msg);

    FkResult _queryLayers(std::shared_ptr<FkMessage> msg);

    FkResult _crop(std::shared_ptr<FkMessage> &msg);

    FkResult _cropLayer(std::shared_ptr<FkMessage> &msg);

private:
    std::shared_ptr<FkSessionClient> client;
    std::shared_ptr<FkGraphicMolecule> molecule;
    std::shared_ptr<FkEngine> renderEngine = nullptr;
};


#endif //FK_GRAPHIC_FKIMAGEENGINE_H
