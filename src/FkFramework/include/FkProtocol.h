/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_FRAMEWORK_FKPROTOCOL_H
#define FK_FRAMEWORK_FKPROTOCOL_H

#include "FkObject.h"

FK_ABS_CLASS FkProtocol FK_EXTEND FkObject {
public:
    typedef uint16_t Type;
public:
    FkProtocol(Type type);

    FkProtocol(const FkProtocol &o);

    virtual ~FkProtocol();

    virtual Type getType();

private:
    Type type;
};


#endif //FK_FRAMEWORK_FKPROTOCOL_H
