/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKCALCTRANSPROTO_H
#define FK_GRAPHIC_FKCALCTRANSPROTO_H

#include "FkProtocol.h"
#include "FkIntVec2.h"

FK_CLASS FkMeasureTransProto FK_EXTEND FkProtocol {
public:
    FkMeasureTransProto();

    FkMeasureTransProto(const FkMeasureTransProto &o);

    virtual ~FkMeasureTransProto();

public:
    FkIntVec2 value;
};

#endif //FK_GRAPHIC_FKCALCTRANSPROTO_H