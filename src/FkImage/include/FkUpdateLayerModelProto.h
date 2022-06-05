/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2022-06-04 23:53:17
*/

#ifndef FK_IMAGE_FKUPDATELAYERMODELPROTO_H
#define FK_IMAGE_FKUPDATELAYERMODELPROTO_H

#include "FkProtocol.h"
#include "FkModelInterface.h"

FK_SUPER_CLASS(FkUpdateLayerModelProto, FkProtocol) {
FK_DEF_CLASS_TYPE_FUNC(FkUpdateLayerModelProto)

public:
    FkUpdateLayerModelProto(std::shared_ptr<FkModelInterface> modelInterface);

    FkUpdateLayerModelProto(const FkUpdateLayerModelProto &o);

    virtual ~FkUpdateLayerModelProto();

public:
    std::shared_ptr<FkModelInterface> modelInterface = nullptr;
};

#endif //FK_IMAGE_FKUPDATELAYERMODELPROTO_H