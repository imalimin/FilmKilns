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

class FkObjectA : public FkObject {
public:
    FkObjectA() {
        FK_MARK_SUPER
    };
};

class FkObjectB : public FkObjectA {
public:
    FkObjectB() : classGuard(clsType) {
    };
    const FkClassTypeGuard<FkObjectB> classGuard;
};
class FkObjectC : public FkObject {
public:
    FkObjectC() : classGuard(clsType) {
    };
    const FkClassTypeGuard<FkObjectC> classGuard;
};

TEST(FkClassHierarchyTest, hierarchy2) {
    auto a = std::make_shared<FkObjectA>();
    auto b = std::make_shared<FkObjectB>();
    auto c = std::make_shared<FkObjectC>();
    std::shared_ptr<FkObjectA> d = b;
    EXPECT_TRUE(FK_CLASS_TYPE_EQUALS2(b, a));
    EXPECT_FALSE(FK_CLASS_TYPE_EQUALS2(a, b));
    EXPECT_FALSE(FK_CLASS_TYPE_EQUALS2(b, c));
    EXPECT_TRUE(FK_CLASS_TYPE_EQUALS2(d, b));
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
