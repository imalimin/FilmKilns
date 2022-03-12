/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: ${author}
* CREATE TIME: 2022-3-12 9:59:53
*/

#ifndef FK_IMAGE_FKQUERYLAYERSPROTO_H
#define FK_IMAGE_FKQUERYLAYERSPROTO_H

#include "FkProtocol.h"
#include "FkGraphicLayer.h"
#include <vector>

FK_CLASS FkQueryLayersProto FK_EXTEND FkProtocol {
public:
    FkQueryLayersProto();

    FkQueryLayersProto(const FkQueryLayersProto &o);

    virtual ~FkQueryLayersProto();

public:
    std::vector<std::shared_ptr<FkGraphicLayer>> layers;
};

#endif //FK_IMAGE_FKQUERYLAYERSPROTO_H