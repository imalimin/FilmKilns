/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICRENDERATOM_H
#define FK_GRAPHIC_FKGRAPHICRENDERATOM_H

#include "FkSimpleAtom.h"
#include "FkGraphicLayer.h"
#include "FkMaterialEntity.h"
#include "FkRenderDeviceRequest.h"

FK_SUPER_CLASS(FkGraphicRenderAtom, FkSimpleAtom) {
FK_DEF_CLASS_TYPE_FUNC(FkGraphicRenderAtom)

public:
    FkGraphicRenderAtom();

    FkGraphicRenderAtom(const FkGraphicRenderAtom &o) = delete;

    virtual ~FkGraphicRenderAtom();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

    virtual void onConnect(std::shared_ptr<FkConnectChain> chain) override;

private:
    FkResult _onRenderRequest(std::shared_ptr<FkProtocol> p);

    FkResult _onCopyLayer(std::shared_ptr<FkProtocol> &p);

    FkResult _onReadPixels(std::shared_ptr<FkProtocol> &p);

    FkResult _onSetRenderCanvasTexFunc(std::shared_ptr<FkProtocol> &p);

    std::shared_ptr<FkMaterialEntity> _makeRenderMaterials(std::shared_ptr<FkGraphicLayer> &layer);

    FkResult _makeDrawCanvasRequest(std::shared_ptr<FkGraphicLayer> &canvas,
                                std::shared_ptr<FkRenderDeviceRequest> &request);

    FkResult _makeDrawPointsRequest(std::shared_ptr<FkGraphicLayer> &layer,
                                    std::shared_ptr<FkRenderDeviceRequest> &request);

    FkResult _makeDrawPathsRequest(std::shared_ptr<FkGraphicLayer> &layer,
                                   std::shared_ptr<FkRenderDeviceRequest> &request);

    FkResult _makeDrawTextRequest(std::shared_ptr<FkGraphicLayer> &layer,
                                   std::shared_ptr<FkRenderDeviceRequest> &request);

private:
    std::function<void(uint32_t, FkSize, int64_t)> mRenderCanvasTexFunc = nullptr;
};

#endif //FK_GRAPHIC_FKGRAPHICRENDERATOM_H
