/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_FRAMEWORK_FKHELLOPROT_H
#define FK_FRAMEWORK_FKHELLOPROT_H

#include "FkProtocol.h"

FK_CLASS FkHelloProt FK_EXTEND FkProtocol {
public:
    FkHelloProt();

    FkHelloProt(const FkHelloProt &o);

    virtual ~FkHelloProt();
};


#endif //FK_FRAMEWORK_FKHELLOPROT_H
