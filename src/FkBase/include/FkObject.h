/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKOBJECT_H
#define FK_BASE_FKOBJECT_H

#include "Object.h"
#include "FkDefinition.h"

FK_CLASS FkObject FK_EXTEND Object {
public:
    FkObject(const char *name);

    virtual ~FkObject();

private:
    std::string name;
};


#endif //FK_BASE_FKOBJECT_H
