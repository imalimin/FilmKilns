/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICCANVASQUARK_H
#define FK_GRAPHIC_FKGRAPHICCANVASQUARK_H

#include "FkQuark.h"
#include "FkGraphicLayer.h"
#include "FkSize.h"

FK_CLASS FkGraphicCanvasQuark FK_EXTEND FkQuark {
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

private:
    FkResult _onUpdate(std::shared_ptr<FkProtocol> p);

    FkResult _onRenderRequest(std::shared_ptr<FkProtocol> p);

    FkResult _onQueryCanvasSize(std::shared_ptr<FkProtocol> p);

    FkResult _onMeasureTrans(std::shared_ptr<FkProtocol> p);

    FkResult _onWithCanvasSize(std::shared_ptr<FkProtocol> p);

private:
    float _getViewScale(std::shared_ptr<FkGraphicLayer> layer, FkSize &targetSize);

private:
    std::shared_ptr<FkGraphicLayer> canvas;
};

#endif //FK_GRAPHIC_FKGRAPHICCANVASQUARK_H