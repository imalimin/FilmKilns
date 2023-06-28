/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKWINSIZEPROTO_H
#define FK_GRAPHIC_FKWINSIZEPROTO_H

#include "FkProtocol.h"
#include "FkSize.h"

FK_SUPER_CLASS(FkWinSizeProto, FkProtocol) {
FK_DEF_CLASS_TYPE_FUNC(FkWinSizeProto)

public:
    FkWinSizeProto();

    FkWinSizeProto(const FkWinSizeProto &o);

    virtual ~FkWinSizeProto();

public:
    FkSize winSize;
};

#endif //FK_GRAPHIC_FKWINSIZEPROTO_H