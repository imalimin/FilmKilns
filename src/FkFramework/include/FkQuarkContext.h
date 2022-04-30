/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-1 23:51:27
*/

#ifndef FK_FRAMEWORK_FKQUARKCONTEXT_H
#define FK_FRAMEWORK_FKQUARKCONTEXT_H

#include "FkEntity.h"

FK_SUPER_CLASS(FkQuarkContext, FkEntity) {
FK_DEF_CLASS_TYPE_FUNC(FkQuarkContext)

public:
    FkQuarkContext();

    FkQuarkContext(const FkQuarkContext &o) = delete;

    virtual ~FkQuarkContext();
};

#endif //FK_FRAMEWORK_FKQUARKCONTEXT_H