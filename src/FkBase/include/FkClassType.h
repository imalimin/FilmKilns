/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKCLASSTYPE_H
#define FK_BASE_FKCLASSTYPE_H

#include <typeinfo>

#define FK_CLASS_OF(NAME) \
NAME ## _Class            \

#define FK_CLASS_TYPE(NAME) FK_CLASS_OF(NAME)::type

#define FK_DEF_CLASS_TYPE(NAME, SUPER)                                   \
namespace FK_CLASS_OF(NAME) {                                               \
class Type {};                                                           \
extern const char *name;                                                 \
extern const FkClassType type;                                           \
}                                                                        \

#define FK_IMPL_CLASS_TYPE(NAME, SUPER)                                  \
const char *FK_CLASS_OF(NAME)::name = #NAME;                                \
const FkClassType FK_CLASS_OF(NAME)::type(name, typeid(FK_CLASS_OF(NAME)::Type).hash_code(), FK_CLASS_OF(SUPER)::type); \

#define FK_SUPER_TEMPLATE_CLASS(NAME, SUPER, ...)                        \
FK_DEF_CLASS_TYPE(NAME, SUPER)                                           \
template<__VA_ARGS__>                                                    \
class NAME : public SUPER                                                \

#define FK_SUPER_TEMPLATE_CLASS_IMPL(NAME, SUPER)                        \
FK_DEF_CLASS_TYPE(NAME, SUPER)                                           \
class NAME : public SUPER                                                \


#define FK_SUPER_CLASS(NAME, SUPER)                                      \
FK_DEF_CLASS_TYPE(NAME, SUPER)                                           \
class NAME : public SUPER                                                \

#define FK_DEF_CLASS_TYPE_FUNC(NAME)        \
public:                                     \
virtual const FkClassType &getClassType() override { \
    return FK_CLASS_OF(NAME)::type;            \
}                                           \

#define FK_INSTANCE_OF(obj, CLASS) (obj->getClassType().is(FK_CLASS_OF(CLASS)::type))

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
