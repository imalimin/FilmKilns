/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-27 14:59:49
*/

#ifndef FK_GRAPHIC_FKRENDERMVPQUARK_H
#define FK_GRAPHIC_FKRENDERMVPQUARK_H

#include "FkQuark.h"
#include "FkMVPMatrix.h"
#include "FkMaterialEntity.h"
#include "FkTransEntity.h"
#include "FkSize.h"

FK_CLASS FkRenderMvpQuark FK_EXTEND FkQuark {
public:
    FkRenderMvpQuark();

    virtual ~FkRenderMvpQuark();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

private:
    FkResult _onRender(std::shared_ptr<FkProtocol> &p);

private:
    FkResult _setRotation(std::shared_ptr<FkMVPMatrix> &matrix,
                          std::shared_ptr<FkTransEntity> &transEntity);

    FkResult _setScale(std::shared_ptr<FkMVPMatrix> &matrix,
                       std::shared_ptr<FkTransEntity> &transEntity,
                       FkSize &targetSize,
                       bool reverseY);

    FkResult _setTranslate(std::shared_ptr<FkMVPMatrix> &matrix,
                           std::shared_ptr<FkTransEntity> &transEntity);

    std::shared_ptr<FkMVPMatrix> _calcMatrix(std::shared_ptr<FkTransEntity> transEntity, FkSize &targetSize, bool reverseY);
};

#endif //FK_GRAPHIC_FKRENDERMVPQUARK_H