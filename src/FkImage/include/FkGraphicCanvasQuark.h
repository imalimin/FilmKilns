/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICCANVASQUARK_H
#define FK_GRAPHIC_FKGRAPHICCANVASQUARK_H

#include "FkGraphicLayerQuark.h"
#include "FkGraphicLayer.h"
#include "FkSize.h"

FK_CLASS FkGraphicCanvasQuark FK_EXTEND FkGraphicLayerQuark {
public:
    FkGraphicCanvasQuark();

    FkGraphicCanvasQuark(const FkGraphicCanvasQuark &o) = delete;

    virtual ~FkGraphicCanvasQuark();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

    FkResult _onRenderRequest(std::shared_ptr<FkProtocol> p) override;

    FkResult _onPostTranslate(std::shared_ptr<FkProtocol> p) override;

    FkResult _onSetTranslate(std::shared_ptr<FkProtocol> p) override;

    FkResult _onPostScale(std::shared_ptr<FkProtocol> p) override;

    FkResult _onSetScale(std::shared_ptr<FkProtocol> p) override;

    FkResult _onPostRotate(std::shared_ptr<FkProtocol> p) override;

    FkResult _onSetRotate(std::shared_ptr<FkProtocol> p) override;

    FkResult _onCrop(std::shared_ptr<FkProtocol> &p) override;

private:
    FkResult _onQueryCanvasSize(std::shared_ptr<FkProtocol> p);

    FkResult _onWithCanvas(std::shared_ptr<FkProtocol> p);

    FkResult _onDelivery(std::shared_ptr<FkProtocol> p);

    std::shared_ptr<FkGraphicLayer> _getCanvas();

    FkResult _initializeCanvas();

};

#endif //FK_GRAPHIC_FKGRAPHICCANVASQUARK_H