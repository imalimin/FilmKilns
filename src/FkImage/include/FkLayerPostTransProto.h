/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKLAYERTRANSPROTO_H
#define FK_GRAPHIC_FKLAYERTRANSPROTO_H

#include "FkIntVec2Proto.h"
#include "FkIntVec2.h"

FK_SUPER_CLASS(FkLayerPostTransProto, FkIntVec2Proto) {
FK_DEF_CLASS_TYPE_FUNC(FkLayerPostTransProto)

public:
    FkLayerPostTransProto();

    FkLayerPostTransProto(const FkLayerPostTransProto &o);

    virtual ~FkLayerPostTransProto();

public:
    FkID layer = FK_ID_NONE;
};

#endif //FK_GRAPHIC_FKLAYERTRANSPROTO_H