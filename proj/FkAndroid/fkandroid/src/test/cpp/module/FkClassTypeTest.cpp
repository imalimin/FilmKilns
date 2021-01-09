/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "gtest/gtest.h"
#include "FkCalculatePrt.h"

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
    EXPECT_TRUE(FK_INSTANCE_OF(prt, FkObject));
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
