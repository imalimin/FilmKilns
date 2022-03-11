/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "gtest/gtest.h"
#include "FkRenderEngine.h"
#include "FkImageEngine.h"
#include "FkImageFileEngine.h"

class FkImageFileEngineTest : public testing::Test {
    void SetUp() override {
        std::string workspace = "/storage/emulated/0/Android/data/com.alimin.fk.test/cache/";
        std::shared_ptr<FkEngine> renderEngine = std::make_shared<FkRenderEngine>("RenderEngine");
        imageEngine = std::make_shared<FkImageEngine>(renderEngine, workspace, "ImageEngine");
        EXPECT_EQ(imageEngine->create(), FK_OK);
        EXPECT_EQ(imageEngine->start(), FK_OK);
        std::shared_ptr<FkEngine> engine = imageEngine;
        fileEngine = std::make_shared<FkImageFileEngine>(engine, "FileEngine");
        EXPECT_EQ(fileEngine->create(), FK_OK);
        EXPECT_EQ(fileEngine->start(), FK_OK);
    }

    void TearDown() override {
        EXPECT_EQ(fileEngine->stop(), FK_OK);
        EXPECT_EQ(fileEngine->destroy(), FK_OK);
        EXPECT_EQ(imageEngine->stop(), FK_OK);
        EXPECT_EQ(imageEngine->destroy(), FK_OK);
        fileEngine = nullptr;
        imageEngine = nullptr;
    }

protected:
    std::shared_ptr<FkImageEngine> imageEngine = nullptr;
    std::shared_ptr<FkImageFileEngine> fileEngine = nullptr;
};

TEST_F(FkImageFileEngineTest, saveFileLayer) {
    std::string path = "/storage/emulated/0/Android/data/com.alimin.fk.test/cache/images/image_0.jpg";
    EXPECT_GT(imageEngine->newLayerWithFile(path), 0);
    std::string draftFile = "/sdcard/000009";
    EXPECT_EQ(fileEngine->save(draftFile), FK_OK);
}