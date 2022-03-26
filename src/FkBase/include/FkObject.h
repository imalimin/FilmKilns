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

FK_CLASS FkObject {
public:
    FkObject();

    virtual ~FkObject();

    virtual const FkClassType &getClassType() const;

    virtual std::string toString();

protected:
    FkClassType clsType;
};


#endif //FK_BASE_FKOBJECT_H
