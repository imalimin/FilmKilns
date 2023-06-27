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

static void testCreate(std::shared_ptr<FkAtom> &atom);
static void testStart(std::shared_ptr<FkAtom> &atom);
static void testStop(std::shared_ptr<FkAtom> &atom);
static void testDestroy(std::shared_ptr<FkAtom> &atom);

class FkAtomTest : public testing::Test {
    void SetUp() override {
        atom = std::make_shared<FkIncrease2Atom>();
        testCreate(atom);
        testStart(atom);
    }

    void TearDown() override {
        testStop(atom);
        testDestroy(atom);
        atom = nullptr;
    }

protected:
    std::shared_ptr<FkAtom> atom = nullptr;
};

class FkSimpleAtomTest : public testing::Test {
    void SetUp() override {
        atom = std::make_shared<FkSimpleIncrease2Atom>();
        testCreate(atom);
        testStart(atom);
    }

    void TearDown() override {
        testStop(atom);
        testDestroy(atom);
        atom = nullptr;
    }

protected:
    std::shared_ptr<FkAtom> atom = nullptr;
};

static void testCreate(std::shared_ptr<FkAtom> &atom) {
    auto session = FkSession::with({FkOnCreatePrt_Class::type.getId(), FkOnCreatePrt_Class::type.getName()});
    EXPECT_EQ(session->connectTo(atom), FK_OK);
    EXPECT_EQ(session->open(), FK_OK);
    FkLocalClient client;
    EXPECT_EQ(client.send(session, std::make_shared<FkOnCreatePrt>(nullptr)), FK_OK);
    EXPECT_NE(client.send(session, std::make_shared<FkOnCreatePrt>(nullptr)), FK_OK);
    EXPECT_EQ(session->close(), FK_OK);
}

static void testStart(std::shared_ptr<FkAtom> &atom) {
    auto session = FkSession::with({FkOnStartPrt_Class::type.getId(), FkOnStartPrt_Class::type.getName()});
    EXPECT_EQ(session->connectTo(atom), FK_OK);
    EXPECT_EQ(session->open(), FK_OK);
    FkLocalClient client;
    EXPECT_EQ(client.send(session, std::make_shared<FkOnStartPrt>()), FK_OK);
    EXPECT_NE(client.send(session, std::make_shared<FkOnStartPrt>()), FK_OK);
    EXPECT_EQ(session->close(), FK_OK);
}

static void testStop(std::shared_ptr<FkAtom> &atom) {
    auto session = FkSession::with({FkOnStopPrt_Class::type.getId(), FkOnStopPrt_Class::type.getName()});
    EXPECT_EQ(session->connectTo(atom), FK_OK);
    EXPECT_EQ(session->open(), FK_OK);
    FkLocalClient client;
    EXPECT_EQ(client.send(session, std::make_shared<FkOnStopPrt>()), FK_OK);
    EXPECT_NE(client.send(session, std::make_shared<FkOnStopPrt>()), FK_OK);
    EXPECT_EQ(session->close(), FK_OK);
}

static void testDestroy(std::shared_ptr<FkAtom> &atom) {
    auto session = FkSession::with({FkOnDestroyPrt_Class::type.getId(), FkOnDestroyPrt_Class::type.getName()});
    EXPECT_EQ(session->connectTo(atom), FK_OK);
    EXPECT_EQ(session->open(), FK_OK);
    FkLocalClient client;
    EXPECT_EQ(client.send(session, std::make_shared<FkOnDestroyPrt>()), FK_OK);
    EXPECT_NE(client.send(session, std::make_shared<FkOnDestroyPrt>()), FK_OK);
    EXPECT_EQ(session->close(), FK_OK);
}

TEST_F(FkAtomTest, Run) {
    FkLocalClient client;
    auto session = FkSession::with({FkCalculatePrt_Class::type.getId(), FkCalculatePrt_Class::type.getName()});
    EXPECT_EQ(session->connectTo(atom), FK_OK);
    EXPECT_EQ(session->open(), FK_OK);
    auto runPrt = std::make_shared<FkCalculatePrt>();
    runPrt->number = 1.0f;
    EXPECT_EQ(client.send(session, runPrt), FK_OK);
    EXPECT_EQ(runPrt->number, 2.0f);
}

TEST_F(FkSimpleAtomTest, Run) {
    FkLocalClient client;
    auto session = FkSession::with({FkCalculatePrt_Class::type.getId(), FkCalculatePrt_Class::type.getName()});
    EXPECT_EQ(session->connectTo(atom), FK_OK);
    EXPECT_EQ(session->open(), FK_OK);
    auto runPrt = std::make_shared<FkCalculatePrt>();
    runPrt->number = 1.0f;
    EXPECT_EQ(client.send(session, runPrt), FK_OK);
    EXPECT_EQ(runPrt->number, 2.0f);
}

