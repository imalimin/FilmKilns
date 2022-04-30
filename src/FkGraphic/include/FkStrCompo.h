/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: ${author}
* CREATE TIME: 2022-3-12 9:37:07
*/

#ifndef FK_GRAPHIC_FKSTRCOMPO_H
#define FK_GRAPHIC_FKSTRCOMPO_H

#include "FkComponent.h"

FK_SUPER_CLASS(FkStrCompo, FkComponent) {
FK_DEF_CLASS_TYPE_FUNC(FkStrCompo)

public:
    FkStrCompo(std::string &str);

    FkStrCompo(const FkStrCompo &o);

    virtual ~FkStrCompo();

public:
    std::string str;
};

#endif //FK_GRAPHIC_FKSTRCOMPO_H