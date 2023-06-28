/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKOBJECT_H
#define FK_BASE_FKOBJECT_H

#include "FkDefinition.h"
#include "FkClassType.h"

namespace FkObject_Class {
    class Type {};
    extern const char *name;
    extern const FkClassType type;
}

class FkObject {
public:
    virtual const FkClassType &getClassType() {
        return FkObject_Class::type;
    }

public:
    FkObject();

    virtual ~FkObject();

    virtual std::string toString();
};


#endif //FK_BASE_FKOBJECT_H
