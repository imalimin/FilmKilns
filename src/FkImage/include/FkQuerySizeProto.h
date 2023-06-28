/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKQUERYSIZEPROTO_H
#define FK_GRAPHIC_FKQUERYSIZEPROTO_H

#include "FkSetSizeProto.h"

FK_SUPER_CLASS(FkQuerySizeProto, FkSetSizeProto) {
FK_DEF_CLASS_TYPE_FUNC(FkQuerySizeProto)

public:
    FkQuerySizeProto();

    FkQuerySizeProto(const FkQuerySizeProto &o);

    virtual ~FkQuerySizeProto();
};

#endif //FK_GRAPHIC_FKQUERYSIZEPROTO_H