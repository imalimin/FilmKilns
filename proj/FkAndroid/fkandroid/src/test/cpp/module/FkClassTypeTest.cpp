/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include <list>
#include <string>
#include "gtest/gtest.h"
#include "FkCalculatePrt.h"
#include "FkClassType.h"

TEST(FkClassTypeTests, Equals) {
    FkCalculatePrt prot;
    FkObject obj;
    EXPECT_TRUE(FK_CLASS_TYPE_EQUALS(prot, obj));
}

TEST(FkClassTypeTests, EqualsReverse) {
    FkCalculatePrt prot;
    FkObject obj;
    EXPECT_FALSE(FK_CLASS_TYPE_EQUALS(obj, prot));
}

TEST(FkClassTypeTests, Equals2) {
    auto prt = std::make_shared<FkCalculatePrt>();
    auto obj = std::make_shared<FkObject>();
    EXPECT_TRUE(FK_CLASS_TYPE_EQUALS2(prt, obj));
}

TEST(FkClassTypeTests, Instance) {
    auto prt = std::make_shared<FkCalculatePrt>();
    auto type = FkClassType::type<FkObject>();
//    EXPECT_TRUE(FK_INSTANCE_OF(prt, FkObject));
}

class FkClassHierarchy {
public:
    FkClassHierarchy(const char *name) : name(name), hashValue() {
        id = hashValue(name);
    }

    FkClassHierarchy(const char *name, const FkClassHierarchy &o) : name(name), hashValue() {
        id = hashValue(name);
        next = &o;
    }

    FkClassHierarchy(const FkClassHierarchy &o) = delete;

    virtual ~FkClassHierarchy() {}

    bool is(const FkClassHierarchy &o) const {
        if (this == &o) {
            return true;
        }
        auto cur = const_cast<FkClassHierarchy *>(this);
        do {
            if (cur->id == o.id) {
                return true;
            }
        } while ((cur = const_cast<FkClassHierarchy *>(cur->next)) != nullptr);
        return false;
    }

private:
    size_t id;
    const char *name;
    const std::hash<std::string> hashValue;
    const FkClassHierarchy *next = nullptr;
};

#define FK_CLASS_DEF(NAME, SUPER) \
namespace NAME ## _Info { \
const char *name = #NAME; \
FkClassHierarchy hierarchy(name, SUPER ## _Info::hierarchy); \
} \
class NAME : public SUPER         \

namespace FkObjectA_Info {
    const char *name = "FkObjectA";
    FkClassHierarchy hierarchy(name);
}

class FkObjectA : public FkObject {
public:
    virtual const FkClassHierarchy &classType() {
        return FkObjectA_Info::hierarchy;
    }

public:
    FkObjectA() {
    };
};

FK_CLASS_DEF(FkObjectB, FkObjectA) {
public:
    virtual const FkClassHierarchy &classType() override {
        return FkObjectB_Info::hierarchy;
    }

public:
    FkObjectB() : FkObjectA() {
    }
};

FK_CLASS_DEF(FkObjectC, FkObjectB) {
public:
    virtual const FkClassHierarchy &classType() override {
        return FkObjectC_Info::hierarchy;
    }

public:
    FkObjectC() : FkObjectB() {
    }
};

TEST(FkClassHierarchyTest, hierarchy2) {
    auto a = std::make_shared<FkObjectA>();
    auto b = std::make_shared<FkObjectB>();
    auto c = std::make_shared<FkObjectC>();
    std::shared_ptr<FkObjectA> d = b;

    EXPECT_TRUE(a->classType().is(FkObjectA_Info::hierarchy));
    EXPECT_FALSE(a->classType().is(FkObjectB_Info::hierarchy));

    EXPECT_TRUE(b->classType().is(FkObjectA_Info::hierarchy));
    EXPECT_TRUE(b->classType().is(FkObjectB_Info::hierarchy));
    EXPECT_FALSE(b->classType().is(FkObjectC_Info::hierarchy));

    EXPECT_TRUE(d->classType().is(FkObjectA_Info::hierarchy));
    EXPECT_TRUE(d->classType().is(FkObjectB_Info::hierarchy));
    EXPECT_FALSE(d->classType().is(FkObjectC_Info::hierarchy));

    EXPECT_TRUE(d->classType().is(a->classType()));
    EXPECT_TRUE(d->classType().is(b->classType()));
    EXPECT_FALSE(d->classType().is(c->classType()));
}

//TEST(FkClassTypeTests, SuperTrack) {
//    FkCalculatePrt prot;
//    EXPECT_STREQ(prot.getClassType().toString().c_str(), "8FkObject:<-10FkProtocol:<-15FkCalculatePrt");
//}
//
//TEST(FkClassTypeTests, HashCode) {
//    FkCalculatePrt prot;
//    EXPECT_EQ(prot.getClassType().hashCode(), -5469057871962402202L);
//}
