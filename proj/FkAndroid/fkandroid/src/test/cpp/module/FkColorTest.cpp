/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "gtest/gtest.h"
#include "FkColor.h"

TEST(FkColorTest, Int) {
    FkColor color = FkColor::makeFromRGBA8(255, 255, 255, 125);
    EXPECT_EQ(color.toInt(), 0x7dffffff);
    EXPECT_EQ(FkColor::makeFrom(color.toInt()).uRed(), color.uRed());
    EXPECT_EQ(FkColor::makeFrom(color.toInt()).uGreen(), color.uGreen());
    EXPECT_EQ(FkColor::makeFrom(color.toInt()).uBlue(), color.uBlue());
    EXPECT_EQ(FkColor::makeFrom(color.toInt()).uAlpha(), color.uAlpha());
}

TEST(FkColorTest, Equals) {
    FkColor color = FkColor::makeFromRGBA8(255, 255, 255, 125);
    EXPECT_TRUE(FkColor::makeFrom(color.toInt()).equals(color));
}
