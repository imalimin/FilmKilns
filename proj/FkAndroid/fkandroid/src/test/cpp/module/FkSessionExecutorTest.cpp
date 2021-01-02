/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "gtest/gtest.h"
#include "FkSession.h"
#include "FkIncreaseQuark.h"
#include "FkDecreaseQuark.h"
#include "FkDivideQuark.h"
#include "FkCalculateProt.h"
#include "FkSessionExecutor.h"

TEST(FkSessionExecutorTest, Execute) {
    auto session = FkSession::with(std::make_shared<FkCalculateProt>());

    auto increase = std::make_shared<FkIncreaseQuark>();
    increase->onCreate();
    auto decrease = std::make_shared<FkDecreaseQuark>();
    decrease->onCreate();
    auto divide = std::make_shared<FkDivideQuark>();
    divide->onCreate();

    EXPECT_EQ(FK_CONNECT_TO(session, increase), FK_OK);
    EXPECT_EQ(FK_CONNECT_TO(session, decrease), FK_OK);
    EXPECT_EQ(FK_CONNECT_TO(session, divide), FK_OK);
    increase->onStart();
    decrease->onStart();
    divide->onStart();

    FkSessionExecutor executor;
    auto protocol = std::make_shared<FkCalculateProt>();
    protocol->number = 1.0f;
    EXPECT_EQ(executor.send(session, protocol), FK_OK);
    EXPECT_EQ(protocol->number, 1.5f);

    protocol->number = 2.0f;
    EXPECT_EQ(executor.send(session, protocol), FK_OK);
    EXPECT_EQ(protocol->number, 2.0f);

    divide->onStop();
    decrease->onStop();
    increase->onStop();

    divide->onDestroy();
    decrease->onDestroy();
    increase->onDestroy();

    session->close();
}
