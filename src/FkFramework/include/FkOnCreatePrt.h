/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_FRAMEWORK_FKONCREATEPROT_H
#define FK_FRAMEWORK_FKONCREATEPROT_H

#include "FkProtocol.h"
#include "FkQuarkContext.h"

FK_SUPER_CLASS(FkOnCreatePrt, FkProtocol) {
FK_DEF_CLASS_TYPE_FUNC(FkOnCreatePrt)

public:
    FkOnCreatePrt();

    FkOnCreatePrt(const FkOnCreatePrt &o);

    virtual ~FkOnCreatePrt();

public:
    std::shared_ptr<FkQuarkContext> context = nullptr;
};

#endif //FK_FRAMEWORK_FKONCREATEPROT_H
