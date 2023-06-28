/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2022-07-01 11:09:09
*/

#ifndef FK_GRAPHIC_FKVIEWPORTMATCOMPO_H
#define FK_GRAPHIC_FKVIEWPORTMATCOMPO_H

#include "FkMatCompo.h"

FK_SUPER_CLASS(FkViewportMatCompo, FkMatCompo) {
FK_DEF_CLASS_TYPE_FUNC(FkViewportMatCompo)

public:
    FkViewportMatCompo();

    FkViewportMatCompo(const FkViewportMatCompo &o);

    virtual ~FkViewportMatCompo();
};

#endif //FK_GRAPHIC_FKVIEWPORTMATCOMPO_H