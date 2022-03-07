/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "gtest/gtest.h"
#include "FkMessage.h"

TEST(FkSourceTest, CapacityLimitMin) {
    std::vector<std::shared_ptr<FkMessage>> vec;
    for (int i = 0; i < 49; ++i) {
        int desc = 0;
        auto msg = FkMessageAllocator::getInstance()->alloc(desc);
        vec.push_back(msg);
    }
    EXPECT_EQ(FkMessageAllocator::getInstance()->size(), 49);
    EXPECT_EQ(FkMessageAllocator::getInstance()->capacity(), 49);

    vec.erase(vec.begin());
    EXPECT_EQ(FkMessageAllocator::getInstance()->size(), 48);
    EXPECT_EQ(FkMessageAllocator::getInstance()->capacity(), 49);

    vec.clear();
    EXPECT_EQ(FkMessageAllocator::getInstance()->size(), 0);
    EXPECT_EQ(FkMessageAllocator::getInstance()->capacity(), 49);
}

TEST(FkSourceTest, CapacityLimitMax) {
    std::vector<std::shared_ptr<FkMessage>> vec;
    for (int i = 0; i < 100; ++i) {
        int desc = 0;
        auto msg = FkMessageAllocator::getInstance()->alloc(desc);
        vec.push_back(msg);
    }
    EXPECT_EQ(FkMessageAllocator::getInstance()->size(), 100);

    vec.clear();
    EXPECT_EQ(FkMessageAllocator::getInstance()->size(), 0);
    EXPECT_EQ(FkMessageAllocator::getInstance()->capacity(), 50);
}