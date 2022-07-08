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
#include "FkColor.h"
#include "FkMaterialEntity.h"
#include "FkDeviceEntity.h"
#include "FkGraphicWindow.h"
#include "FkBuffer.h"
#include "FkRenderDeviceRequest.h"

FK_SUPER_CLASS(FkRenderEngine, FkEngine) {
FK_DEF_CLASS_TYPE_FUNC(FkRenderEngine)

public:
    FkRenderEngine(std::string name);

    FkRenderEngine(const FkRenderEngine &o) = delete;

    virtual ~FkRenderEngine();

    FkResult renderDevice(std::shared_ptr<FkRenderDeviceRequest> &request);

    FkResult renderDevice(std::shared_ptr<FkMaterialEntity> &materials, std::shared_ptr<FkDeviceEntity> &device);

    std::shared_ptr<FkMaterialCompo> addMaterial();

    FkResult removeMaterial(std::shared_ptr<FkMaterialCompo> &material);

    FkResult updateMaterial(std::shared_ptr<FkMaterialCompo> &material, FkSize size, FkColor color);

    FkResult updateMaterialWithBitmap(std::shared_ptr<FkMaterialCompo> &material, FkSize size, std::shared_ptr<FkBuffer> buf);

    FkResult updateWindow(std::shared_ptr<FkGraphicWindow> win);

protected:
    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

    FkResult _onRender(std::shared_ptr<FkMessage> &msg);

    FkResult _onAddMaterial(std::shared_ptr<FkMessage> msg);

    FkResult _onRemoveMaterial(std::shared_ptr<FkMessage> msg);

    FkResult _onUpdateMaterial(std::shared_ptr<FkMessage> &msg);

    FkResult _onUpdateMaterialWithBitmap(std::shared_ptr<FkMessage> &msg);

    FkResult _onUpdateWindow(std::shared_ptr<FkMessage> &msg);

private:
    static const FkID MSG_RENDER_DEVICE;
    std::shared_ptr<FkSessionClient> client;
    std::shared_ptr<FkQuark> molecule;
};

#endif //FK_GRAPHIC_FKRENDERENGINE_H