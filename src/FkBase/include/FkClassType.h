/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKCLASSTYPE_H
#define FK_BASE_FKCLASSTYPE_H

#include <list>
#include <string>

class FkClassType {
public:
    FkClassType();

    FkClassType(const FkClassType &o);

    virtual ~FkClassType();

    void super(const char *name);

    friend bool operator==(const FkClassType &o0, const FkClassType &o1);

private:
    std::list<std::string> extends;
};


#endif //FK_BASE_FKCLASSTYPE_H
