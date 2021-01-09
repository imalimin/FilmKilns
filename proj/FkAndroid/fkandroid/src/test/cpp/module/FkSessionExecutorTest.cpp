/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "gtest/gtest.h"
#include "FkSession.h"
#include "FkIncreaseQuark.h"
#include "FkOnCreateProt.h"
#include "FkSessionExecutor.h"

TEST(FkSessionExecutorTest, Execute) {
    auto quark = std::make_shared<FkIncreaseQuark>();
    auto session = FkSession::with(std::make_shared<FkOnCreateProt>());
    EXPECT_EQ(session->connectTo(quark), FK_OK);
    EXPECT_EQ(session->open(), FK_OK);
    FkSessionExecutor executor;
    EXPECT_EQ(executor.send(session, std::make_shared<FkOnCreateProt>()), FK_OK);
    EXPECT_EQ(session->close(), FK_OK);
}
