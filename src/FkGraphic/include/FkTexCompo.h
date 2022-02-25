/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: ${author}
* CREATE TIME: 2022-2-25 13:30:26
*/

#ifndef FK_GRAPHIC_FKTEXCOMPO_H
#define FK_GRAPHIC_FKTEXCOMPO_H

#include "FkComponent.h"

FK_CLASS FkTexCompo FK_EXTEND FkComponent {
public:
    FkTexCompo();

    FkTexCompo(const FkTexCompo &o);

    virtual ~FkTexCompo();
};

#endif //FK_GRAPHIC_FKTEXCOMPO_H