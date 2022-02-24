/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "gtest/gtest.h"
#include "FkRenderEngine.h"
#include "FkTestDefine.h"

TEST(FkRenderTest, Livecycle) {
    auto engine = std::make_shared<FkRenderEngine>("RenderEngine");
    EXPECT_EQ(engine->create(), FK_OK);
    EXPECT_NE(engine->create(), FK_OK);
    EXPECT_EQ(engine->start(), FK_OK);
    EXPECT_NE(engine->start(), FK_OK);
    EXPECT_EQ(engine->stop(), FK_OK);
    EXPECT_NE(engine->stop(), FK_OK);
    EXPECT_EQ(engine->destroy(), FK_OK);
    EXPECT_NE(engine->destroy(), FK_OK);
}

TEST(FkRenderTest, Render) {
    auto engine = std::make_shared<FkRenderEngine>("RenderEngine");
    EXPECT_NE(engine->render(), FK_OK);
    EXPECT_EQ(engine->create(), FK_OK);
    EXPECT_NE(engine->render(), FK_OK);
    EXPECT_EQ(engine->start(), FK_OK);
    EXPECT_EQ(engine->render(), FK_OK);
    EXPECT_EQ(engine->stop(), FK_OK);
    EXPECT_NE(engine->render(), FK_OK);
    EXPECT_EQ(engine->destroy(), FK_OK);
}

TEST(FkRenderTest, NewMaterial) {
    FK_NEW_INSTANCE(engine, FkRenderEngine, "RenderEngine")
    auto material = engine->newMaterial();
    EXPECT_EQ(material->isUseless(), false);
    EXPECT_EQ(engine->updateMaterial(material, FkSize(32, 32), FkColor::white()), FK_OK);
    FK_DELETE_INSTANCE(engine)
}
