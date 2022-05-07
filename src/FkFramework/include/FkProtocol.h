/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_FRAMEWORK_FKPROTOCOL_H
#define FK_FRAMEWORK_FKPROTOCOL_H

#include "FkObject.h"

FK_SUPER_CLASS(FkProtocol, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkProtocol)

public:
    typedef size_t Type;

    struct Desc {
        FkProtocol::Type type;
        const char *name;
    };
public:
    FkProtocol();

    FkProtocol(const FkProtocol &o);

    virtual ~FkProtocol();

    virtual Type getType();

};


#endif //FK_FRAMEWORK_FKPROTOCOL_H
