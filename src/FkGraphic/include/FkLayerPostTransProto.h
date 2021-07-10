/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKLAYERTRANSPROTO_H
#define FK_GRAPHIC_FKLAYERTRANSPROTO_H

#include "FkProtocol.h"
#include "FkIntVec2.h"

FK_CLASS FkLayerPostTransProto FK_EXTEND FkProtocol {
public:
    FkLayerPostTransProto();

    FkLayerPostTransProto(const FkLayerPostTransProto &o);

    virtual ~FkLayerPostTransProto();

public:
    FkID layer = FK_ID_NONE;
    FkIntVec2 value;
};

#endif //FK_GRAPHIC_FKLAYERTRANSPROTO_H