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
#include "FkClassType2.h"

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
    const static FkClassType2<FkObjectA> classType;
};
const FkClassType2<FkObjectA> FkObjectA::classType = FkClassType2<FkObjectA>();

class FkObjectB : FkObjectA {
public:
    const static FkClassType2<FkObjectB> classType;
};
const FkClassType2<FkObjectB> FkObjectB::classType = FkClassType2<FkObjectB>(FkObjectA::classType.hierarchy);

class FkObjectC : public FkObject {
public:
    const static FkClassType2<FkObjectC> classType;
};
const FkClassType2<FkObjectC> FkObjectC::classType = FkClassType2<FkObjectC>();

TEST(FkClassHierarchyTest, hierarchy) {
    EXPECT_TRUE(FkObjectB::classType == FkObjectA::classType);
    EXPECT_FALSE(FkObjectB::classType == FkObjectC::classType);
}

TEST(FkClassHierarchyTest, hierarchy2) {
    auto a = std::make_shared<FkObjectA>();
    auto b = std::make_shared<FkObjectB>();
    auto c = std::make_shared<FkObjectC>();
    EXPECT_TRUE(b->classType == a->classType);
    EXPECT_FALSE(a->classType == b->classType);
    EXPECT_FALSE(b->classType == c->classType);

    EXPECT_TRUE(a->classType == FkObjectA::classType);
    EXPECT_TRUE(b->classType == FkObjectB::classType);
    EXPECT_TRUE(c->classType == FkObjectC::classType);

    EXPECT_FALSE(a->classType == FkObjectB::classType);
    EXPECT_TRUE(b->classType == FkObjectA::classType);
    EXPECT_FALSE(c->classType == FkObjectB::classType);
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
