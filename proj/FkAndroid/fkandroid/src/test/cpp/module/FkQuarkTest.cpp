/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "gtest/gtest.h"
#include "FkHelloQuark.h"
#include "FkHelloProt.h"
#include "FkSayProt.h"

TEST(FkQuarkTest, Dispatch) {
    FkHelloQuark quark;
    quark.onCreate();
    quark.onStart();

    auto p = std::make_shared<FkHelloProt>();
    EXPECT_EQ(quark.dispatch(p), 0);
}

TEST(FkQuarkTest, DispatchErr) {
    FkHelloQuark quark;
    quark.onCreate();
    quark.onStart();

    auto p = std::make_shared<FkSayProt>();
    EXPECT_NE(quark.dispatch(p), 0);
}