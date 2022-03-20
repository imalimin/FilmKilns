/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-19 17:22:21
*/

#ifndef FK_GRAPHIC_FKPOINTPROCESSQUARK_H
#define FK_GRAPHIC_FKPOINTPROCESSQUARK_H

#include "FkQuark.h"
#include "FkDeviceEntity.h"
#include "FkTexEntity.h"
#include "FkPointVertexCompo.h"

FK_CLASS FkPointProcessQuark FK_EXTEND FkQuark {
public:
    FkPointProcessQuark();

    virtual ~FkPointProcessQuark();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

private:
    FkResult _onRender(std::shared_ptr<FkProtocol> p);

    FkResult _drawPoints(std::shared_ptr<FkPointVertexCompo> &vertexCompo,
                         std::shared_ptr<FkTexEntity> &materials,
                         std::shared_ptr<FkTexDeviceEntity> &device);
};

#endif //FK_GRAPHIC_FKPOINTPROCESSQUARK_H