/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKCLASSTYPE_H
#define FK_BASE_FKCLASSTYPE_H

#include <typeinfo>

#define FK_GET_CLASS_TYPE(NAME) NAME ## _Class::type

class FkClassType {
public:
    FkClassType(const char *_name, size_t _id);

    FkClassType(const char *_name, size_t _id, const FkClassType &o);

    FkClassType(const FkClassType &o) = delete;

    virtual ~FkClassType();

    const char *getName() const;

    size_t getId() const;

    bool is(const FkClassType &o) const;

    friend bool operator==(const FkClassType &o0, const FkClassType &o1);

    friend bool operator!=(const FkClassType &o0, const FkClassType &o1);

private:
    size_t id;
    const char *name;
    const FkClassType *next = nullptr;
};


#endif //FK_BASE_FKCLASSTYPE_H
