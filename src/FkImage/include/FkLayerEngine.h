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

    FkID newLayer();

    FkID newLayerWithColor(FkSize size, FkColor color);

    FkResult setCanvasSize(FkSize size);

    FkResult postTranslate(FkID layer, int32_t dx, int32_t dy);

    FkResult postScale(FkID layer, float dx, float dy);

    FkResult postRotation(FkID layer, FkRational &rational);

    FkResult drawPoint(FkID layer, FkColor color, int32_t x, int32_t y);

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

    FkResult _postScale(std::shared_ptr<FkMessage> msg);

    FkResult _postRotation(std::shared_ptr<FkMessage> msg);

    FkResult _drawPoint(std::shared_ptr<FkMessage> msg);

private:
    static const FkID FK_MSG_NEW_LAYER;
    static const FkID FK_MSG_UPDATE_LAYER_WITH_COLOR;
    static const FkID FK_MSG_SET_SURFACE;
    static const FkID FK_MSG_NOTIFY_RENDER;
    static const FkID FK_MSG_SET_CANVAS_SIZE;
    static const FkID FK_MSG_POST_TRANSLATE;
    static const FkID FK_MSG_POST_SCALE;
    static const FkID FK_MSG_POST_ROTATION;
    static const FkID FK_MSG_DRAW_POINT;
    std::shared_ptr<FkSessionClient> client;
    std::shared_ptr<FkGraphicMolecule> molecule;
    std::shared_ptr<FkEngine> renderEngine = nullptr;
};


#endif //FK_GRAPHIC_FKIMAGEENGINE_H
