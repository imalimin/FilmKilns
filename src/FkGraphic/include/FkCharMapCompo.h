/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-09-02 10:02:37
*/

#ifndef FK_GRAPHIC_FKCHARMAPCOMPO_H
#define FK_GRAPHIC_FKCHARMAPCOMPO_H

#include "FkComponent.h"
#include "FkTextureCharMap.h"

FK_SUPER_CLASS(FkCharMapCompo, FkComponent) {
FK_DEF_CLASS_TYPE_FUNC(FkCharMapCompo)

public:
    FkCharMapCompo(const std::shared_ptr<FkTextureCharMap> &charMap);

    FkCharMapCompo(const FkCharMapCompo &o);

    virtual ~FkCharMapCompo();

public:
    std::shared_ptr<FkTextureCharMap> charMap = nullptr;
};

#endif //FK_GRAPHIC_FKCHARMAPCOMPO_H