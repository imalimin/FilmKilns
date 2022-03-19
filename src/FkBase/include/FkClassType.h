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
#include <typeinfo>

class FkClassType {
public:
    template<class T>
    static FkClassType &type() {
        static FkClassType *type;
        if (type == nullptr) {
            type = new FkClassType();
        }
        put(type->hierarchy, typeid(T));
        return *type;
    };
public:
    FkClassType();

    FkClassType(const FkClassType &o);

    virtual ~FkClassType();

#ifdef __FK_OPTIMIZE_CLASS_TYPE__

    void super(const size_t hashCode);

#else
    void super(const char *name);
#endif

    friend bool operator==(const FkClassType &o0, const FkClassType &o1);

    friend bool operator!=(const FkClassType &o0, const FkClassType &o1);

    std::string toString() const;

    size_t hashCode() const;

    std::string getName() const;

    void put(const std::type_info &info) {
        put(hierarchy, info);
    }

protected:
    static void put(std::list<size_t> &hierarchy, const std::type_info &info) {
#ifdef __FK_OPTIMIZE_CLASS_TYPE__
        hierarchy.emplace_back(info.hash_code());
#else
        hierarchy.emplace_back(info.name());
#endif
    }

public:
#ifdef __FK_OPTIMIZE_CLASS_TYPE__
    std::list<size_t> hierarchy;
#else
    std::list<std::string> hierarchy;
    std::hash<std::string> hashValue;
#endif
};

template<typename T>
class FkClassTypeGuard {
public:
    FkClassTypeGuard(FkClassType &type) {
        type.put(typeid(T));
    }

    ~FkClassTypeGuard() {}
};


#endif //FK_BASE_FKCLASSTYPE_H
