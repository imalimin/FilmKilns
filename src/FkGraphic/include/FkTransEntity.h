/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-20 23:30:10
*/

#ifndef FK_GRAPHIC_FKTRANSENTITY_H
#define FK_GRAPHIC_FKTRANSENTITY_H

#include "FkEntity.h"

FK_CLASS FkTransEntity FK_EXTEND FkEntity {
public:
    FkTransEntity();

    FkTransEntity(const FkTransEntity &o);

    virtual ~FkTransEntity();
};

#endif //FK_GRAPHIC_FKTRANSENTITY_H