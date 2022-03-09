/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "gtest/gtest.h"
#include "FkIncreaseQuark.h"
#include "FkSession.h"
#include "FkLocalClient.h"
#include "FkOnCreatePrt.h"
#include "FkOnDestroyPrt.h"
#include "FkOnStartPrt.h"
#include "FkOnStopPrt.h"
#include "FkCalculatePrt.h"

static void testCreate(std::shared_ptr<FkQuark> &quark);
static void testStart(std::shared_ptr<FkQuark> &quark);
static void testStop(std::shared_ptr<FkQuark> &quark);
static void testDestroy(std::shared_ptr<FkQuark> &quark);

class FkQuarkTest : public testing::Test {
    void SetUp() override {
        quark = std::make_shared<FkIncreaseQuark>();
        testCreate(quark);
        testStart(quark);
    }

    void TearDown() override {
        testStop(quark);
        testDestroy(quark);
        quark = nullptr;
    }

protected:
    std::shared_ptr<FkQuark> quark = nullptr;
};

static void testCreate(std::shared_ptr<FkQuark> &quark) {
    auto session = FkSession::with(std::make_shared<FkOnCreatePrt>());
    EXPECT_EQ(session->connectTo(quark), FK_OK);
    EXPECT_EQ(session->open(), FK_OK);
    FkLocalClient client;
    EXPECT_EQ(client.send(session, std::make_shared<FkOnCreatePrt>()), FK_OK);
    EXPECT_NE(client.send(session, std::make_shared<FkOnCreatePrt>()), FK_OK);
    EXPECT_EQ(session->close(), FK_OK);
}

static void testStart(std::shared_ptr<FkQuark> &quark) {
    auto session = FkSession::with(std::make_shared<FkOnStartPrt>());
    EXPECT_EQ(session->connectTo(quark), FK_OK);
    EXPECT_EQ(session->open(), FK_OK);
    FkLocalClient client;
    EXPECT_EQ(client.send(session, std::make_shared<FkOnStartPrt>()), FK_OK);
    EXPECT_NE(client.send(session, std::make_shared<FkOnStartPrt>()), FK_OK);
    EXPECT_EQ(session->close(), FK_OK);
}

static void testStop(std::shared_ptr<FkQuark> &quark) {
    auto session = FkSession::with(std::make_shared<FkOnStopPrt>());
    EXPECT_EQ(session->connectTo(quark), FK_OK);
    EXPECT_EQ(session->open(), FK_OK);
    FkLocalClient client;
    EXPECT_EQ(client.send(session, std::make_shared<FkOnStopPrt>()), FK_OK);
    EXPECT_NE(client.send(session, std::make_shared<FkOnStopPrt>()), FK_OK);
    EXPECT_EQ(session->close(), FK_OK);
}

static void testDestroy(std::shared_ptr<FkQuark> &quark) {
    auto session = FkSession::with(std::make_shared<FkOnDestroyPrt>());
    EXPECT_EQ(session->connectTo(quark), FK_OK);
    EXPECT_EQ(session->open(), FK_OK);
    FkLocalClient client;
    EXPECT_EQ(client.send(session, std::make_shared<FkOnDestroyPrt>()), FK_OK);
    EXPECT_NE(client.send(session, std::make_shared<FkOnDestroyPrt>()), FK_OK);
    EXPECT_EQ(session->close(), FK_OK);
}

TEST_F(FkQuarkTest, Run) {
    FkLocalClient client;
    auto calSes = FkSession::with(std::make_shared<FkCalculatePrt>());
    EXPECT_EQ(calSes->connectTo(quark), FK_OK);
    EXPECT_EQ(calSes->open(), FK_OK);
    auto prot = std::make_shared<FkCalculatePrt>();
    prot->number = 1.0f;
    EXPECT_EQ(client.send(calSes, prot), FK_OK);
    EXPECT_EQ(calSes->close(), FK_OK);
    EXPECT_EQ(prot->number, 4.0f);
}