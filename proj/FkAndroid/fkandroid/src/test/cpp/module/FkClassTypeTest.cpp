/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "gtest/gtest.h"
#include "FkHelloProt.h"

TEST(FkClassTypeTests, Equals) {
    auto *prot = new FkHelloProt();
    auto *obj = new FkObject();
    ASSERT_TRUE(FK_CLASS_TYPE_EQUALS(*prot, *obj));
    delete obj;
    delete prot;
}
