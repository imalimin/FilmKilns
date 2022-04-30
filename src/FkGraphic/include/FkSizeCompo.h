/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: ${author}
* CREATE TIME: 2022-2-25 13:23:15
*/

#ifndef FK_GRAPHIC_FKSIZECOMPO_H
#define FK_GRAPHIC_FKSIZECOMPO_H

#include "FkComponent.h"
#include "FkSize.h"

FK_SUPER_CLASS(FkSizeCompo, FkComponent) {
FK_DEF_CLASS_TYPE_FUNC(FkSizeCompo)

public:
    FkSizeCompo(FkSize size);

    FkSizeCompo(const FkSizeCompo &o);

    virtual ~FkSizeCompo();

public:
    FkSize size;
};

#endif //FK_GRAPHIC_FKSIZECOMPO_H