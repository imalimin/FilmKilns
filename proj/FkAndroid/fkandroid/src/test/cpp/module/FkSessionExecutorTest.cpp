/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "gtest/gtest.h"
#include "FkSession.h"
#include "FkIncreaseQuark.h"
#include "FkOnCreatePrt.h"
#include "FkLocalClient.h"

TEST(FkSessionExecutorTest, Execute) {
    auto quark = std::make_shared<FkIncreaseQuark>();
    auto session = FkSession::with({FkOnCreatePrt_Class::type.getId(), FkOnCreatePrt_Class::type.getName()});
    EXPECT_EQ(session->connectTo(quark), FK_OK);
    EXPECT_EQ(session->open(), FK_OK);
    FkLocalClient client;
    EXPECT_EQ(client.send(session, std::make_shared<FkOnCreatePrt>()), FK_OK);
    EXPECT_EQ(session->close(), FK_OK);
}
