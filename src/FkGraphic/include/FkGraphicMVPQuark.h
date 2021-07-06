/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICMVPQUARK_H
#define FK_GRAPHIC_FKGRAPHICMVPQUARK_H

#include "FkQuark.h"
#include "FkFloatVec3.h"
#include "FkMVPMatrix.h"
#include "FkGraphicLayer.h"
#include "FkSize.h"

FK_CLASS FkGraphicMVPQuark FK_EXTEND FkQuark {
public:
    FkGraphicMVPQuark();

    FkGraphicMVPQuark(const FkGraphicMVPQuark &o) = delete;

    virtual ~FkGraphicMVPQuark();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

private:
    FkResult _onRenderRequest(std::shared_ptr<FkProtocol> p);

    FkResult _onSetViewSize(std::shared_ptr<FkProtocol> p);

private:
    float _getViewScale(std::shared_ptr<FkGraphicLayer> layer);

    FkResult _setRotation(std::shared_ptr<FkMVPMatrix> matrix,
                          std::shared_ptr<FkGraphicLayer> layer);

    FkResult _setScale(std::shared_ptr<FkMVPMatrix> matrix,
                       std::shared_ptr<FkGraphicLayer> layer);

    FkResult _setTranslate(std::shared_ptr<FkMVPMatrix> matrix,
                           std::shared_ptr<FkGraphicLayer> layer);

private:
    FkSize viewSize;
    kScaleType scaleType = kScaleType::CENTER_CROP;
};

#endif //FK_GRAPHIC_FKGRAPHICMVPQUARK_H