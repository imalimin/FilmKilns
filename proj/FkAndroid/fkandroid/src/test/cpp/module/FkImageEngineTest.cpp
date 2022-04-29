/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "gtest/gtest.h"
#include "FkRenderEngine.h"
#include "FkImageEngine.h"
#include "FkString.h"
#include "FkFileUtils.h"
#include "FkImageModelEngine.h"

#define FK_ANDROID_TEST_CACHE_DIR "/storage/emulated/0/Android/data/com.alimin.fk.test/cache"
#define FK_ANDROID_TEST_CACHE_IMAGE_0 "/storage/emulated/0/Android/data/com.alimin.fk.test/cache/images/image_0.jpg"

class FkImageEngineTest : public testing::Test {
protected:
    virtual void SetUp() override {
        fkpFile = FkString(FK_ANDROID_TEST_CACHE_DIR)
                .append("/draft/")
                .append(FkTimeUtils::getCurrentTimeUS())
                .append(".fkp")
                .toString();
        workspace = FkString(FK_ANDROID_TEST_CACHE_DIR)
                .append("/draft/")
                .append(FkTimeUtils::getCurrentTimeUS())
                .toString();
        auto str = workspace.c_str();
        EXPECT_EQ(FkFileUtils::mkdirs(workspace), FK_OK);
        std::shared_ptr<FkEngine> renderEngine = std::make_shared<FkRenderEngine>("RenderEngine");
        engine = std::make_shared<FkImageEngine>(renderEngine, workspace, "ImageEngine");
        EXPECT_EQ(engine->create(), FK_OK);
        EXPECT_EQ(engine->start(), FK_OK);
    }

    virtual void TearDown() override {
        EXPECT_EQ(engine->stop(), FK_OK);
        EXPECT_EQ(engine->destroy(), FK_OK);
        engine = nullptr;
    }

protected:
    std::string workspace;
    std::string fkpFile;
    std::shared_ptr<FkImageEngine> engine = nullptr;
};

TEST_F(FkImageEngineTest, newLayerWithColor) {
    auto size = FkSize(512, 512);
    auto color = FkColor::white();
    EXPECT_GT(engine->newLayerWithColor(size, color), 0);
}

TEST_F(FkImageEngineTest, newLayerWithFile) {
    std::string path = FK_ANDROID_TEST_CACHE_IMAGE_0;
    EXPECT_GT(engine->newLayerWithFile(path), 0);
}

class FkImageFileEngineTest : public FkImageEngineTest {
protected:
    virtual void SetUp() override {
        FkImageEngineTest::SetUp();
        std::shared_ptr<FkEngine> imageEngine = engine;
        fileEngine = std::make_shared<FkImageModelEngine>(imageEngine, "FileEngine");
        EXPECT_EQ(fileEngine->create(), FK_OK);
        EXPECT_EQ(fileEngine->start(), FK_OK);
    }

    virtual void TearDown() override {
        EXPECT_EQ(fileEngine->stop(), FK_OK);
        EXPECT_EQ(fileEngine->destroy(), FK_OK);
        FkImageEngineTest::TearDown();
        fileEngine = nullptr;
    }

protected:
    std::shared_ptr<FkImageModelEngine> fileEngine = nullptr;
};

TEST_F(FkImageFileEngineTest, saveFkp) {
    std::string path = FK_ANDROID_TEST_CACHE_IMAGE_0;
    EXPECT_GT(engine->newLayerWithFile(path), 0);
    EXPECT_EQ(FkFileUtils::mkdirs(FkFileUtils::parent(fkpFile)), FK_OK);
    EXPECT_EQ(fileEngine->save(fkpFile), FK_OK);
    EXPECT_EQ(fileEngine->load(fkpFile), FK_OK);
}

TEST_F(FkImageFileEngineTest, getLayers) {
    std::string path = FK_ANDROID_TEST_CACHE_IMAGE_0;
    EXPECT_GT(engine->newLayerWithFile(path), 0);
    EXPECT_EQ(FkFileUtils::mkdirs(FkFileUtils::parent(fkpFile)), FK_OK);
    auto callback = [](std::shared_ptr<pb::FkPictureModel> &model) {

    };
    EXPECT_EQ(fileEngine->getLayers(callback), FK_OK);
}