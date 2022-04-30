/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKSETSIZEPROTO_H
#define FK_GRAPHIC_FKSETSIZEPROTO_H

#include "FkProtocol.h"
#include "FkSize.h"

FK_SUPER_CLASS(FkSetSizeProto, FkProtocol) {
FK_DEF_CLASS_TYPE_FUNC(FkSetSizeProto)

public:
    FkSetSizeProto();

    FkSetSizeProto(const FkSetSizeProto &o);

    virtual ~FkSetSizeProto();

public:
    FkSize value;
};

#endif //FK_GRAPHIC_FKSETSIZEPROTO_H