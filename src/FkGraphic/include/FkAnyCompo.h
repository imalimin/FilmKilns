/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-4-9 23:59:09
*/

#ifndef FK_FRAMEWORK_FKANYCOMPO_H
#define FK_FRAMEWORK_FKANYCOMPO_H

#include "FkObject.h"
#include <any>

FK_CLASS FkAnyCompo FK_EXTEND FkObject {
public:
    FkAnyCompo(std::any any);

    FkAnyCompo(const FkAnyCompo &o);

    virtual ~FkAnyCompo();

public:
    std::any any;
};

#endif //FK_FRAMEWORK_FKANYCOMPO_H