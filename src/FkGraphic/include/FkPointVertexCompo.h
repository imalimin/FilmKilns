/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-19 21:05:06
*/

#ifndef FK_GRAPHIC_FKPOINTVERTEXCOMPO_H
#define FK_GRAPHIC_FKPOINTVERTEXCOMPO_H

#include "FkVertexCompo.h"
#include "FkColor.h"

FK_SUPER_CLASS(FkPointVertexCompo, FkVertexCompo) {
FK_DEF_CLASS_TYPE_FUNC(FkPointVertexCompo)

public:
    FkPointVertexCompo();

    FkPointVertexCompo(const FkPointVertexCompo &o);

    virtual ~FkPointVertexCompo();

};

#endif //FK_GRAPHIC_FKPOINTVERTEXCOMPO_H