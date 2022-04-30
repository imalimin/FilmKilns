/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-27 14:40:57
*/

#ifndef FK_GRAPHIC_FKCOORDINATECOMPO_H
#define FK_GRAPHIC_FKCOORDINATECOMPO_H

#include "FkVertexCompo.h"

FK_SUPER_CLASS(FkCoordinateCompo, FkVertexCompo) {
FK_DEF_CLASS_TYPE_FUNC(FkCoordinateCompo)

public:
    FkCoordinateCompo();

    FkCoordinateCompo(const FkCoordinateCompo &o);

    virtual ~FkCoordinateCompo();
};

#endif //FK_GRAPHIC_FKCOORDINATECOMPO_H