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

TEST(FkAtomTest, Create) {
    auto atom = std::make_shared<FkIncrease2Atom>();
    auto session = FkSession::with(std::make_shared<FkOnCreateProt>());
    EXPECT_EQ(session->connectTo(atom), FK_OK);
    EXPECT_EQ(session->open(), FK_OK);
    FkSessionExecutor executor;
    EXPECT_EQ(executor.send(session, std::make_shared<FkOnCreateProt>()), FK_OK);
    EXPECT_EQ(session->close(), FK_OK);
}

TEST(FkAtomTest, CreateError) {
    auto atom = std::make_shared<FkIncrease2Atom>();
    auto session = FkSession::with(std::make_shared<FkOnCreateProt>());
    EXPECT_EQ(session->connectTo(atom), FK_OK);
    EXPECT_EQ(session->open(), FK_OK);
    FkSessionExecutor executor;
    EXPECT_EQ(executor.send(session, std::make_shared<FkOnCreateProt>()), FK_OK);
    EXPECT_NE(executor.send(session, std::make_shared<FkOnCreateProt>()), FK_OK);
    EXPECT_EQ(session->close(), FK_OK);
}

TEST(FkAtomTest, Start) {
    auto atom = std::make_shared<FkIncrease2Atom>();
    auto session = FkSession::with(std::make_shared<FkOnCreateProt>());
    EXPECT_EQ(session->connectTo(atom), FK_OK);
    EXPECT_EQ(session->open(), FK_OK);
    FkSessionExecutor executor;
    EXPECT_EQ(executor.send(session, std::make_shared<FkOnCreateProt>()), FK_OK);

    auto onStartSes = FkSession::with(std::make_shared<FkOnStartProt>());
    EXPECT_EQ(onStartSes->connectTo(atom), FK_OK);
    EXPECT_EQ(onStartSes->open(), FK_OK);
    EXPECT_EQ(executor.send(onStartSes, std::make_shared<FkOnStartProt>()), FK_OK);

    EXPECT_EQ(onStartSes->close(), FK_OK);
    EXPECT_EQ(session->close(), FK_OK);
}

TEST(FkAtomTest, StartError) {
    auto atom = std::make_shared<FkIncreaseQuark>();
    auto session = FkSession::with(std::make_shared<FkOnStartProt>());
    EXPECT_EQ(session->connectTo(atom), FK_OK);
    EXPECT_EQ(session->open(), FK_OK);
    FkSessionExecutor executor;
    EXPECT_NE(executor.send(session, std::make_shared<FkOnStartProt>()), FK_OK);
    EXPECT_EQ(session->close(), FK_OK);
}

TEST(FkAtomTest, Run) {
    FkSessionExecutor executor;
    auto atom = std::make_shared<FkIncrease2Atom>();
    //Live create
    auto onCreateSes = FkSession::with(std::make_shared<FkOnCreateProt>());
    EXPECT_EQ(onCreateSes->connectTo(atom), FK_OK);
    EXPECT_EQ(onCreateSes->open(), FK_OK);
    EXPECT_EQ(executor.send(onCreateSes, std::make_shared<FkOnCreateProt>()), FK_OK);
    EXPECT_EQ(onCreateSes->close(), FK_OK);
    // Live start
    auto onStartSes = FkSession::with(std::make_shared<FkOnStartProt>());
    EXPECT_EQ(onStartSes->connectTo(atom), FK_OK);
    EXPECT_EQ(onStartSes->open(), FK_OK);
    EXPECT_EQ(executor.send(onStartSes, std::make_shared<FkOnStartProt>()), FK_OK);
    EXPECT_EQ(onStartSes->close(), FK_OK);
    //run
    auto runSes = FkSession::with(std::make_shared<FkCalculateProt>());
    EXPECT_EQ(runSes->connectTo(atom), FK_OK);
    EXPECT_EQ(runSes->open(), FK_OK);
    auto runPrt = std::make_shared<FkCalculateProt>();
    runPrt->number = 1.0f;
    EXPECT_EQ(executor.send(runSes, runPrt), FK_OK);
    EXPECT_EQ(runPrt->number, 2.0f);
    //Live stop
    auto onStopSes = FkSession::with(std::make_shared<FkOnStopProt>());
    EXPECT_EQ(onStopSes->connectTo(atom), FK_OK);
    EXPECT_EQ(onStopSes->open(), FK_OK);
    EXPECT_EQ(executor.send(onStopSes, std::make_shared<FkOnStopProt>()), FK_OK);
    EXPECT_EQ(onStopSes->close(), FK_OK);
    //Live destroy
    auto onDestroySes = FkSession::with(std::make_shared<FkOnDestroyProt>());
    EXPECT_EQ(onDestroySes->connectTo(atom), FK_OK);
    EXPECT_EQ(onDestroySes->open(), FK_OK);
    EXPECT_EQ(executor.send(onDestroySes, std::make_shared<FkOnDestroyProt>()), FK_OK);
    EXPECT_EQ(onDestroySes->close(), FK_OK);
}

