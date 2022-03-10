/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "gtest/gtest.h"
#include "FkRenderEngine.h"
#include "FkImageEngine.h"

class FkImageEngineTest : public testing::Test {
    void SetUp() override {
        std::shared_ptr<FkEngine> renderEngine = std::make_shared<FkRenderEngine>("RenderEngine");
        engine = std::make_shared<FkImageEngine>(renderEngine, "ImageEngine");
        EXPECT_EQ(engine->create(), FK_OK);
        EXPECT_EQ(engine->start(), FK_OK);
    }

    void TearDown() override {
        EXPECT_EQ(engine->stop(), FK_OK);
        EXPECT_EQ(engine->destroy(), FK_OK);
        engine = nullptr;
    }

protected:
    std::shared_ptr<FkImageEngine> engine = nullptr;
};

TEST_F(FkImageEngineTest, newLayerWithColor) {
    auto size = FkSize(512, 512);
    auto color = FkColor::white();
    EXPECT_GT(engine->newLayerWithColor(size, color), 0);
}

TEST_F(FkImageEngineTest, newLayerWithFile) {
    std::string path = "/storage/emulated/0/Android/data/com.alimin.fk.test/cache/images/image_0.jpg";
    EXPECT_GT(engine->newLayerWithFile(path), 0);
}
