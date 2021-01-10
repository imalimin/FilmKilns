/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "gtest/gtest.h"
#include "FkSPObject.h"

TEST(FkSPObjectTest, New) {
    auto sp0 = sk_make_sp<FkSPObject>();
    auto sp1 = sk_make_sp<FkSPObject2>();
    sp0->obj = sp1;
    sp1->obj = sp0;
    EXPECT_NE(sp0, nullptr);
    EXPECT_NE(sp1, nullptr);
}