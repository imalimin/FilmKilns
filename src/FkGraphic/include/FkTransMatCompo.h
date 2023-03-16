/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-03-17 00:33:51
*/

#ifndef FK_GRAPHIC_FKTRANSMATCOMPO_H
#define FK_GRAPHIC_FKTRANSMATCOMPO_H

#include "FkMatCompo.h"

FK_SUPER_CLASS(FkTransMatCompo, FkMatCompo) {
FK_DEF_CLASS_TYPE_FUNC(FkTransMatCompo)

public:
    FkTransMatCompo();

    FkTransMatCompo(const FkTransMatCompo &o);

    virtual ~FkTransMatCompo();
};

#endif //FK_GRAPHIC_FKTRANSMATCOMPO_H