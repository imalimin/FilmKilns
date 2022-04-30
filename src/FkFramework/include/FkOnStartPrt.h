/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_FRAMEWORK_FKONSTARTPROT_H
#define FK_FRAMEWORK_FKONSTARTPROT_H

#include "FkProtocol.h"

FK_SUPER_CLASS(FkOnStartPrt, FkProtocol) {
FK_DEF_CLASS_TYPE_FUNC(FkOnStartPrt)

public:
    FkOnStartPrt();

    FkOnStartPrt(const FkOnStartPrt &o);

    virtual ~FkOnStartPrt();

};

#endif //FK_FRAMEWORK_FKONSTARTPROT_H
