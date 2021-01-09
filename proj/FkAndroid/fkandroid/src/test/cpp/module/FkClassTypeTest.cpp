/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "gtest/gtest.h"
#include "FkCalculateProt.h"

TEST(FkClassTypeTests, Equals) {
    FkCalculateProt prot;
    FkObject obj;
    EXPECT_TRUE(FK_CLASS_TYPE_EQUALS(prot, obj));
}

TEST(FkClassTypeTests, EqualsReverse) {
    FkCalculateProt prot;
    FkObject obj;
    EXPECT_FALSE(FK_CLASS_TYPE_EQUALS(obj, prot));
}

TEST(FkClassTypeTests, SuperTrack) {
    FkCalculateProt prot;
    EXPECT_STREQ(prot.getClassType().toString().c_str(), "8FkObject:<-10FkProtocol:<-15FkCalculateProt");
}

TEST(FkClassTypeTests, HashCode) {
    FkCalculateProt prot;
    EXPECT_EQ(prot.getClassType().hashCode(), -5469057871962402202L);
}
