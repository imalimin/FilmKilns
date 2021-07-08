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
#include <functional>
#include "FkDefinition.h"

class FkClassType {
public:
    template<class T>
    static FkClassType &type() {
        static FkClassType *type;
        if (type == nullptr) {
            type = new FkClassType();
        }
#ifdef __FK_OPTIMIZE_CLASS_TYPE__
        type->super(typeid(T).hash_code());
#else
        type->super(typeid(T).name());
#endif
        return *type;
    };
public:
    FkClassType();

    FkClassType(const FkClassType &o);

    virtual ~FkClassType();

#ifdef __FK_OPTIMIZE_CLASS_TYPE__

    void super(const size_t &hashCode);

#else
    void super(const char *name);
#endif

    friend bool operator==(const FkClassType &o0, const FkClassType &o1);

    friend bool operator!=(const FkClassType &o0, const FkClassType &o1);

    std::string toString() const;

    size_t hashCode() const;

    std::string getName() const;

private:
#ifdef __FK_OPTIMIZE_CLASS_TYPE__
    std::list<size_t> extends;
#else
    std::list<std::string> extends;
    std::hash<std::string> hashValue;
#endif
};


#endif //FK_BASE_FKCLASSTYPE_H
