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
#include "FkImgEngineSettings.h"
#include "FkOffWindow.h"

#define FK_ANDROID_TEST_CACHE_DIR "/storage/emulated/0/Android/data/com.alimin.fk.test/cache"
#define FK_ANDROID_TEST_TEMP_FILE "/storage/emulated/0/Android/data/com.alimin.fk.test/cache/000000.png"
#define FK_ANDROID_TEST_CACHE_IMAGE_0 "/storage/emulated/0/Android/data/com.alimin.fk.test/cache/images/image_0.jpg"
#define FK_ANDROID_TEST_IMAGE_POS "/storage/emulated/0/Android/data/com.alimin.fk.test/cache/images/image_pos.png"

static bool testColor(std::shared_ptr<FkImageEngine> engine, int32_t x, int32_t y, FkColor expect);

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
        auto renderSettings = std::make_shared<FkEngineSettings>();
        renderSettings->enableEngineThread = false;
        renderEngine->setSettings(renderSettings);

        engine = std::make_shared<FkImageEngine>(renderEngine, workspace, "ImageEngine");
        auto imgEngineSettings = std::make_shared<FkImgEngineSettings>();
        imgEngineSettings->enableEngineThread = false;
        engine->setSettings(imgEngineSettings);

        EXPECT_EQ(engine->create(), FK_OK);
        EXPECT_EQ(engine->start(), FK_OK);
        auto win = std::make_shared<FkOffWindow>(1440, 2676);
        EXPECT_EQ(engine->setSurface(win, static_cast<int32_t>(kScaleType::CENTER_INSIDE)), FK_OK);
    }

    virtual void TearDown() override {
        EXPECT_EQ(engine->stop(), FK_OK);
        EXPECT_EQ(engine->destroy(), FK_OK);
        engine = nullptr;
    }

public:
    void render() {
        EXPECT_EQ(engine->notifyRender(), FK_OK);
    }

    FkID addImagePosLayer() {
        auto layerId = engine->newLayerWithFile(FK_ANDROID_TEST_IMAGE_POS);
        EXPECT_GT(layerId, 0);
        return layerId;
    }

    void setTestCanvasSize() {
        EXPECT_EQ(engine->setCanvasSize(FkSize(540, 640)), FK_OK);
    }

    void setTestPosition(FkID layerId) {
        EXPECT_EQ(engine->postTranslate(layerId, -120, 300), FK_OK);
    }

    void setTestScale(FkID layerId) {
        EXPECT_EQ(engine->postScale(layerId, 1.8f, 1.8f), FK_OK);
    }

    void setTestRotate(FkID layerId) {
        FkRational rational(1, 1);
        EXPECT_EQ(engine->postRotation(layerId, rational), FK_OK);
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

TEST_F(FkImageEngineTest, Position) {
    auto layerId = addImagePosLayer();
    setTestCanvasSize();
    setTestPosition(layerId);
    render();
    EXPECT_TRUE(testColor(engine, 225, 432, FkColor::white()));
    EXPECT_TRUE(testColor(engine, 140, 259, FkColor::red()));
    EXPECT_TRUE(testColor(engine, 368, 486, FkColor::green()));
}

TEST_F(FkImageEngineTest, Scale) {
    auto layerId = addImagePosLayer();
    setTestCanvasSize();
    setTestScale(layerId);
    render();
    EXPECT_TRUE(testColor(engine, 270, 320, FkColor::white()));
    EXPECT_TRUE(testColor(engine, 117, 8, FkColor::red()));
    EXPECT_TRUE(testColor(engine, 527, 417, FkColor::green()));
}

TEST_F(FkImageEngineTest, Rotate) {
    auto layerId = addImagePosLayer();
    setTestCanvasSize();
    setTestRotate(layerId);
    render();
    EXPECT_TRUE(testColor(engine, 270, 320, FkColor::white()));
    EXPECT_TRUE(testColor(engine, 79, 298, FkColor::red()));
    EXPECT_TRUE(testColor(engine, 392, 229, FkColor::green()));
//    EXPECT_EQ(engine->save(FK_ANDROID_TEST_TEMP_FILE), FK_OK);
}

TEST_F(FkImageEngineTest, MvpAndCrop) {
    auto layerId = addImagePosLayer();
    setTestCanvasSize();
    setTestPosition(layerId);
    setTestScale(layerId);
    setTestRotate(layerId);
    render();
    EXPECT_TRUE(testColor(engine, 396, 497, FkColor::white()));
    EXPECT_TRUE(testColor(engine, 51, 458, FkColor::red()));

    FkIntRect rect(133, 1100, 856, 1703);
    EXPECT_EQ(engine->crop(rect), FK_OK);
    render();
    EXPECT_TRUE(testColor(engine, 0, 224, FkColor::red()));
//    EXPECT_EQ(engine->save(FK_ANDROID_TEST_TEMP_FILE), FK_OK);
}

class FkImageFileEngineTest : public FkImageEngineTest {
protected:
    virtual void SetUp() override {
        FkImageEngineTest::SetUp();
        std::shared_ptr<FkEngine> imageEngine = engine;
        fileEngine = std::make_shared<FkImageModelEngine>(imageEngine, "FileEngine");
        auto modelSettings = std::make_shared<FkImgEngineSettings>();
        modelSettings->enableEngineThread = false;
        fileEngine->setSettings(modelSettings);
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

static bool testColor(std::shared_ptr<FkImageEngine> engine, int32_t x, int32_t y, FkColor expect) {
    FkIntVec2 pos(x, y);
    FkSize size(1,1);
    FkColor color;
    auto ret = engine->readPixels(Fk_CANVAS_ID, pos, size,
                                  [&color](std::shared_ptr<FkBuffer> buf, FkSize size) {
        EXPECT_TRUE(buf != nullptr);
        color = FkColor::makeFromRGBA8(buf->data()[0], buf->data()[1],
                                       buf->data()[2], buf->data()[3]);
    });
    EXPECT_EQ(ret, FK_OK);
    return color.equals(expect);
}