/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_FRAMEWORK_FKONDESTROYPROT_H
#define FK_FRAMEWORK_FKONDESTROYPROT_H

#include "FkProtocol.h"

FK_SUPER_CLASS(FkOnDestroyPrt, FkProtocol) {
FK_DEF_CLASS_TYPE_FUNC(FkOnDestroyPrt)

public:
    FkOnDestroyPrt();

    FkOnDestroyPrt(const FkOnDestroyPrt &o);

    virtual ~FkOnDestroyPrt();

};

#endif //FK_FRAMEWORK_FKONDESTROYPROT_H
