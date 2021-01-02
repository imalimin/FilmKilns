/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "gtest/gtest.h"
#include "FkIncreaseQuark.h"

TEST(FkQuarkTest, Create) {
    auto quark = std::make_shared<FkIncreaseQuark>();
    quark->onCreate();
    quark->onStart();
    EXPECT_EQ(0, 0);
}