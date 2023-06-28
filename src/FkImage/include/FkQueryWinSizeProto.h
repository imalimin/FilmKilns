/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-27 19:04:09
*/

#ifndef FK_IMAGE_FKQUERYWINSIZEPROTO_H
#define FK_IMAGE_FKQUERYWINSIZEPROTO_H

#include "FkWinSizeProto.h"

FK_SUPER_CLASS(FkQueryWinSizeProto, FkWinSizeProto) {
FK_DEF_CLASS_TYPE_FUNC(FkQueryWinSizeProto)

public:
    FkQueryWinSizeProto();

    FkQueryWinSizeProto(const FkQueryWinSizeProto &o);

    virtual ~FkQueryWinSizeProto();
};

#endif //FK_IMAGE_FKQUERYWINSIZEPROTO_H