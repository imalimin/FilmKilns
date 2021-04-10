/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "gtest/gtest.h"
#include "FkSession.h"
#include "FkLocalClient.h"
#include "FkIncrease2Atom.h"
#include "FkCalculatePrt.h"
#include "FkSimpleIncrease2Atom.h"

TEST(FkAtomTest, Create) {
    auto atom = std::make_shared<FkIncrease2Atom>();
    auto session = FkSession::with(std::make_shared<FkOnCreatePrt>());
    EXPECT_EQ(session->connectTo(atom), FK_OK);
    EXPECT_EQ(session->open(), FK_OK);
    FkLocalClient client;
    EXPECT_EQ(client.send(session, std::make_shared<FkOnCreatePrt>()), FK_OK);
    EXPECT_EQ(session->close(), FK_OK);
}

TEST(FkAtomTest, CreateError) {
    auto atom = std::make_shared<FkIncrease2Atom>();
    auto session = FkSession::with(std::make_shared<FkOnCreatePrt>());
    EXPECT_EQ(session->connectTo(atom), FK_OK);
    EXPECT_EQ(session->open(), FK_OK);
    FkLocalClient client;
    EXPECT_EQ(client.send(session, std::make_shared<FkOnCreatePrt>()), FK_OK);
    EXPECT_NE(client.send(session, std::make_shared<FkOnCreatePrt>()), FK_OK);
    EXPECT_EQ(session->close(), FK_OK);
}

TEST(FkAtomTest, Start) {
    auto atom = std::make_shared<FkIncrease2Atom>();
    auto session = FkSession::with(std::make_shared<FkOnCreatePrt>());
    EXPECT_EQ(session->connectTo(atom), FK_OK);
    EXPECT_EQ(session->open(), FK_OK);
    FkLocalClient client;
    EXPECT_EQ(client.send(session, std::make_shared<FkOnCreatePrt>()), FK_OK);

    auto onStartSes = FkSession::with(std::make_shared<FkOnStartPrt>());
    EXPECT_EQ(onStartSes->connectTo(atom), FK_OK);
    EXPECT_EQ(onStartSes->open(), FK_OK);
    EXPECT_EQ(client.send(onStartSes, std::make_shared<FkOnStartPrt>()), FK_OK);

    EXPECT_EQ(onStartSes->close(), FK_OK);
    EXPECT_EQ(session->close(), FK_OK);
}

TEST(FkAtomTest, StartError) {
    auto atom = std::make_shared<FkIncreaseQuark>();
    auto session = FkSession::with(std::make_shared<FkOnStartPrt>());
    EXPECT_EQ(session->connectTo(atom), FK_OK);
    EXPECT_EQ(session->open(), FK_OK);
    FkLocalClient client;
    EXPECT_NE(client.send(session, std::make_shared<FkOnStartPrt>()), FK_OK);
    EXPECT_EQ(session->close(), FK_OK);
}

TEST(FkAtomTest, Run) {
    FkLocalClient client;
    auto atom = std::make_shared<FkIncrease2Atom>();
    //Live create
    auto onCreateSes = FkSession::with(std::make_shared<FkOnCreatePrt>());
    EXPECT_EQ(onCreateSes->connectTo(atom), FK_OK);
    EXPECT_EQ(onCreateSes->open(), FK_OK);
    EXPECT_EQ(client.send(onCreateSes, std::make_shared<FkOnCreatePrt>()), FK_OK);
    EXPECT_EQ(onCreateSes->close(), FK_OK);
    // Live start
    auto onStartSes = FkSession::with(std::make_shared<FkOnStartPrt>());
    EXPECT_EQ(onStartSes->connectTo(atom), FK_OK);
    EXPECT_EQ(onStartSes->open(), FK_OK);
    EXPECT_EQ(client.send(onStartSes, std::make_shared<FkOnStartPrt>()), FK_OK);
    EXPECT_EQ(onStartSes->close(), FK_OK);
    //run
    auto runSes = FkSession::with(std::make_shared<FkCalculatePrt>());
    EXPECT_EQ(runSes->connectTo(atom), FK_OK);
    EXPECT_EQ(runSes->open(), FK_OK);
    auto runPrt = std::make_shared<FkCalculatePrt>();
    runPrt->number = 1.0f;
    EXPECT_EQ(client.send(runSes, runPrt), FK_OK);
    EXPECT_EQ(runPrt->number, 2.0f);
    //Live stop
    auto onStopSes = FkSession::with(std::make_shared<FkOnStopPrt>());
    EXPECT_EQ(onStopSes->connectTo(atom), FK_OK);
    EXPECT_EQ(onStopSes->open(), FK_OK);
    EXPECT_EQ(client.send(onStopSes, std::make_shared<FkOnStopPrt>()), FK_OK);
    EXPECT_EQ(onStopSes->close(), FK_OK);
    //Live destroy
    auto onDestroySes = FkSession::with(std::make_shared<FkOnDestroyPrt>());
    EXPECT_EQ(onDestroySes->connectTo(atom), FK_OK);
    EXPECT_EQ(onDestroySes->open(), FK_OK);
    EXPECT_EQ(client.send(onDestroySes, std::make_shared<FkOnDestroyPrt>()), FK_OK);
    EXPECT_EQ(onDestroySes->close(), FK_OK);
}

TEST(FkSimpleAtomTest, Run) {
    FkLocalClient client;
    auto atom = std::make_shared<FkSimpleIncrease2Atom>();
    //Live create
    auto onCreateSes = FkSession::with(std::make_shared<FkOnCreatePrt>());
    EXPECT_EQ(onCreateSes->connectTo(atom), FK_OK);
    EXPECT_EQ(onCreateSes->open(), FK_OK);
    EXPECT_EQ(client.send(onCreateSes, std::make_shared<FkOnCreatePrt>()), FK_OK);
    EXPECT_EQ(onCreateSes->close(), FK_OK);
    // Live start
    auto onStartSes = FkSession::with(std::make_shared<FkOnStartPrt>());
    EXPECT_EQ(onStartSes->connectTo(atom), FK_OK);
    EXPECT_EQ(onStartSes->open(), FK_OK);
    EXPECT_EQ(client.send(onStartSes, std::make_shared<FkOnStartPrt>()), FK_OK);
    EXPECT_EQ(onStartSes->close(), FK_OK);
    //run
    auto runSes = FkSession::with(std::make_shared<FkCalculatePrt>());
    EXPECT_EQ(runSes->connectTo(atom), FK_OK);
    EXPECT_EQ(runSes->open(), FK_OK);
    auto runPrt = std::make_shared<FkCalculatePrt>();
    runPrt->number = 1.0f;
    EXPECT_EQ(client.send(runSes, runPrt), FK_OK);
    EXPECT_EQ(runPrt->number, 2.0f);
    //Live stop
    auto onStopSes = FkSession::with(std::make_shared<FkOnStopPrt>());
    EXPECT_EQ(onStopSes->connectTo(atom), FK_OK);
    EXPECT_EQ(onStopSes->open(), FK_OK);
    EXPECT_EQ(client.send(onStopSes, std::make_shared<FkOnStopPrt>()), FK_OK);
    EXPECT_EQ(onStopSes->close(), FK_OK);
    //Live destroy
    auto onDestroySes = FkSession::with(std::make_shared<FkOnDestroyPrt>());
    EXPECT_EQ(onDestroySes->connectTo(atom), FK_OK);
    EXPECT_EQ(onDestroySes->open(), FK_OK);
    EXPECT_EQ(client.send(onDestroySes, std::make_shared<FkOnDestroyPrt>()), FK_OK);
    EXPECT_EQ(onDestroySes->close(), FK_OK);
}

