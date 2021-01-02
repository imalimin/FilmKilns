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
    EXPECT_EQ(quark->onCreate(), FK_OK);
}

TEST(FkQuarkTest, CreateError) {
    auto quark = std::make_shared<FkIncreaseQuark>();
    quark->onCreate();
    EXPECT_NE(quark->onCreate(), FK_OK);
}

TEST(FkQuarkTest, Start) {
    auto quark = std::make_shared<FkIncreaseQuark>();
    quark->onCreate();
    EXPECT_EQ(quark->onStart(), FK_OK);
}

TEST(FkQuarkTest, StartError) {
    auto quark = std::make_shared<FkIncreaseQuark>();
    quark->onCreate();
    quark->onStart();
    EXPECT_NE(quark->onStart(), FK_OK);
}

TEST(FkQuarkTest, Stop) {
    auto quark = std::make_shared<FkIncreaseQuark>();
    quark->onCreate();
    quark->onStart();
    EXPECT_EQ(quark->onStop(), FK_OK);
}

TEST(FkQuarkTest, StopError) {
    auto quark = std::make_shared<FkIncreaseQuark>();
    quark->onCreate();
    quark->onStart();
    quark->onStop();
    EXPECT_NE(quark->onStop(), FK_OK);
}

TEST(FkQuarkTest, Destroy) {
    auto quark = std::make_shared<FkIncreaseQuark>();
    quark->onCreate();
    quark->onStart();
    quark->onStop();
    EXPECT_EQ(quark->onDestroy(), FK_OK);
}

TEST(FkQuarkTest, DestroyError) {
    auto quark = std::make_shared<FkIncreaseQuark>();
    quark->onCreate();
    quark->onStart();
    quark->onStop();
    quark->onDestroy();
    EXPECT_NE(quark->onDestroy(), FK_OK);
}