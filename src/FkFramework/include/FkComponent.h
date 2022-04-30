/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-20 16:57:35
*/

#ifndef FK_FRAMEWORK_FKCOMPONENT_H
#define FK_FRAMEWORK_FKCOMPONENT_H

#include "FkObject.h"

FK_SUPER_CLASS(FkComponent, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkComponent)

public:
    FkComponent();

    FkComponent(const FkComponent &o);

    virtual ~FkComponent();
};

#endif //FK_FRAMEWORK_FKCOMPONENT_H