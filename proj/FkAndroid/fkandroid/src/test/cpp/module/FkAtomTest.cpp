/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "gtest/gtest.h"
#include "FkSession.h"
#include "FkSessionExecutor.h"
#include "FkIncrease2Atom.h"
#include "FkCalculateProt.h"

TEST(FkAtomTest, Live) {
    auto atom = std::make_shared<FkIncrease2Atom>();
    EXPECT_EQ(atom->onCreate(), FK_OK);
    EXPECT_EQ(atom->onStart(), FK_OK);
    EXPECT_EQ(atom->onStop(), FK_OK);
    EXPECT_EQ(atom->onDestroy(), FK_OK);
}

TEST(FkAtomTest, Run) {
    auto session = FkSession::with<FkCalculateProt>(std::make_shared<FkCalculateProt>());
    auto atom = std::make_shared<FkIncrease2Atom>();
    EXPECT_EQ(atom->onCreate(), FK_OK);

    EXPECT_EQ(FK_CONNECT_TO(session, atom), FK_OK);
    EXPECT_EQ(atom->onStart(), FK_OK);
    EXPECT_EQ(session->open(), FK_OK);

    auto protocol = std::make_shared<FkCalculateProt>();
    protocol->number = 2.0f;
    FkSessionExecutor executor;
    EXPECT_EQ(executor.send(session, protocol), FK_OK);
    EXPECT_EQ(protocol->number, 8.0f);
    protocol->number = 10.0f;
    EXPECT_EQ(executor.send(session, protocol), FK_OK);
    EXPECT_EQ(protocol->number, 16.0f);

    EXPECT_EQ(session->close(), FK_OK);
    EXPECT_EQ(atom->onStop(), FK_OK);
    EXPECT_EQ(atom->onDestroy(), FK_OK);
}

