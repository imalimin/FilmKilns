/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_FRAMEWORK_FKONSTOPPROT_H
#define FK_FRAMEWORK_FKONSTOPPROT_H

#include "FkProtocol.h"

FK_CLASS FkOnStopPrt FK_EXTEND FkProtocol {
public:
    FkOnStopPrt();

    FkOnStopPrt(const FkOnStopPrt &o);

    virtual ~FkOnStopPrt();

};

#endif //FK_FRAMEWORK_FKONSTOPPROT_H
