/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICLAYERQUARK_H
#define FK_GRAPHIC_FKGRAPHICLAYERQUARK_H

#include "FkQuark.h"
#include "FkGraphicLayer.h"
#include "FkSize.h"
#include <list>
#include <map>

FK_CLASS FkGraphicLayerQuark FK_EXTEND FkQuark {
public:
    FkGraphicLayerQuark();

    FkGraphicLayerQuark(const FkGraphicLayerQuark &o) = delete;

    virtual ~FkGraphicLayerQuark();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

private:
    FkResult _onNewLayer(std::shared_ptr<FkProtocol> p);

    FkResult _onMeasureTrans(std::shared_ptr<FkProtocol> p);

protected:
    std::shared_ptr<FkGraphicLayer> newLayerEntity();

    FkResult _onUpdateLayer(std::shared_ptr<FkProtocol> p);

    virtual FkResult _onRenderRequest(std::shared_ptr<FkProtocol> p);

    virtual FkResult _onPostTranslate(std::shared_ptr<FkProtocol> p);

    virtual FkResult _onSetTranslate(std::shared_ptr<FkProtocol> p);

    virtual FkResult _onPostScale(std::shared_ptr<FkProtocol> p);

    virtual FkResult _onSetScale(std::shared_ptr<FkProtocol> p);

    virtual FkResult _onPostRotate(std::shared_ptr<FkProtocol> p);

    virtual FkResult _onSetRotate(std::shared_ptr<FkProtocol> p);

    FkResult _onDrawPoint(std::shared_ptr<FkProtocol> p);

    FkResult _onQueryLayers(std::shared_ptr<FkProtocol> p);

private:
    void _setupVertex(std::shared_ptr<FkGraphicLayer> layer);

    bool _isExistLayer(FkID id);

    FkID _generateId(FkID expectId);

protected:
    std::map<FkID, std::shared_ptr<FkGraphicLayer>> layers;
    FkID lastId = FK_ID_NONE;
};


#endif //FK_GRAPHIC_FKGRAPHICLAYERQUARK_H
