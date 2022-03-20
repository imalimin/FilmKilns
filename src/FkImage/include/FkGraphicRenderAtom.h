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

FK_CLASS FkGraphicRenderAtom FK_EXTEND FkSimpleAtom {
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

    std::shared_ptr<FkMaterialEntity> _makeRenderMaterials(std::shared_ptr<FkGraphicLayer> &layer);

    FkResult _drawCanvas2Screen(std::shared_ptr<FkGraphicLayer> &canvas);

    FkResult _drawPoints(std::shared_ptr<FkGraphicLayer> &layer);
};

#endif //FK_GRAPHIC_FKGRAPHICRENDERATOM_H
