/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "gtest/gtest.h"
#include "FkRenderEngine.h"
#include "FkFuncCompo.h"
#include "FkBitmap.h"
#include "FkIntVec2.h"
#include "FkVertexCompo.h"
#include "FkCoordinateCompo.h"
#include "FkMatCompo.h"
#include "FkMVPMatrix.h"
#include "FkIntVec2.h"
#include "FkTexEntity.h"

class FkRenderEngineTest : public testing::Test {
    void SetUp() override {
        engine = std::make_shared<FkRenderEngine>("RenderEngine");
        auto renderSettings = std::make_shared<FkEngineSettings>();
        renderSettings->enableEngineThread = false;
        engine->setSettings(renderSettings);
        EXPECT_EQ(engine->create(), FK_OK);
        EXPECT_NE(engine->create(), FK_OK);
        EXPECT_EQ(engine->start(), FK_OK);
        EXPECT_NE(engine->start(), FK_OK);
    }

    void TearDown() override {
        EXPECT_EQ(engine->stop(), FK_OK);
        EXPECT_NE(engine->stop(), FK_OK);
        EXPECT_EQ(engine->destroy(), FK_OK);
        EXPECT_NE(engine->destroy(), FK_OK);
        engine = nullptr;
    }

protected:
    std::shared_ptr<FkRenderEngine> engine = nullptr;
};

TEST(FkRenderEngineTest_, Render) {
    auto engine = std::make_shared<FkRenderEngine>("RenderEngine");
    auto dst = std::make_shared<FkMaterialCompo>(FK_ID_NONE);
    std::shared_ptr<FkMaterialEntity> material = std::make_shared<FkTexEntity>(dst);
    auto device = std::make_shared<FkDeviceEntity>(dst);
    EXPECT_NE(engine->renderDevice(material, device), FK_OK);
    EXPECT_EQ(engine->create(), FK_OK);
    material = std::make_shared<FkTexEntity>(dst);
    device = std::make_shared<FkDeviceEntity>(dst);
    EXPECT_NE(engine->renderDevice(material, device), FK_OK);
    EXPECT_EQ(engine->start(), FK_OK);
    material = std::make_shared<FkTexEntity>(dst);
    device = std::make_shared<FkDeviceEntity>(dst);
    EXPECT_EQ(engine->renderDevice(material, device), FK_OK);
    EXPECT_EQ(engine->stop(), FK_OK);
    material = std::make_shared<FkTexEntity>(dst);
    device = std::make_shared<FkDeviceEntity>(dst);
    EXPECT_NE(engine->renderDevice(material, device), FK_OK);
    EXPECT_EQ(engine->destroy(), FK_OK);
}

TEST_F(FkRenderEngineTest, NewAndRemoveMaterial) {
    auto src = engine->addMaterial();
    EXPECT_EQ(src->isInvalid(), false);
    EXPECT_EQ(engine->removeMaterial(src), FK_OK);
}

static std::shared_ptr<FkMaterialEntity> makeMaterials(std::shared_ptr<FkMaterialCompo> &material,
                                                       FkSize &size,
                                                       FkIntVec2 trans) {
    auto materials = std::make_shared<FkTexEntity>(material);
    float pos[]{
            -size.getWidth() / 2.0f, -size.getHeight() / 2.0f,//LEFT,BOTTOM
            size.getWidth() / 2.0f, -size.getHeight() / 2.0f,//RIGHT,BOTTOM
            -size.getWidth() / 2.0f, size.getHeight() / 2.0f,//LEFT,TOP
            size.getWidth() / 2.0f, size.getHeight() / 2.0f//RIGHT,TOP
    };
    float coordinate[]{
            0.0f, 0.0f,//LEFT,BOTTOM
            1.0f, 0.0f,//RIGHT,BOTTOM
            0.0f, 1.0f,//LEFT,TOP
            1.0f, 1.0f//RIGHT,TOP
    };
    int bytePerVertex = sizeof(float);
    int countOfVertex = 4;
    int countPerVertex = 2;
    auto vertex = std::make_shared<FkVertexCompo>();
    auto coord = std::make_shared<FkCoordinateCompo>();
    vertex->setup(countOfVertex, countPerVertex, bytePerVertex, pos);
    coord->setup(countOfVertex, countPerVertex, bytePerVertex, coordinate);
    materials->addComponent(vertex);
    materials->addComponent(coord);
    auto matCompo = std::make_shared<FkMatCompo>();
    auto mat = std::make_shared<FkMVPMatrix>(FkMVPMatrix::kProjType::ORTHO);
    FkFloatVec3 transVec3(trans.x, trans.y, 0.0f);
    mat->setTranslate(transVec3);
    mat->calc();
    matCompo->value = mat;
    materials->addComponent(matCompo);
    return materials;
}

static void testColor(std::shared_ptr<FkRenderEngine> &engine,
                      std::shared_ptr<FkMaterialCompo> &src,
                      FkSize &size,
                      FkIntVec2 &pos,
                      FkColor &color) {
    int width = size.getWidth();
    int height = size.getHeight();

    auto buf = FkBuffer::alloc(width * height * 4);
    memset(buf->data(), 125, buf->capacity());
    auto promise = std::make_shared<std::promise<int>>();
    std::shared_ptr<FkDeviceEntity> device = std::make_shared<FkBufDeviceEntity>(buf);
    device->addComponent(std::make_shared<FkFuncCompo>([promise, buf, &size]() {
        promise->set_value(FK_OK);
    }));
    auto materials = makeMaterials(src, size, FkIntVec2(0, 0));
    EXPECT_EQ(engine->renderDevice(materials, device), FK_OK);
    EXPECT_EQ(promise->get_future().get(), FK_OK);
    // Center
    int index = pos.y / 2 * pos.x * 4 + pos.x / 2 * 4;
    auto red = buf->data()[index + 0];
    auto green = buf->data()[index + 1];
    auto blue = buf->data()[index + 2];
    auto alpha = buf->data()[index + 3];
    EXPECT_EQ(red, color.uRed());
    EXPECT_EQ(green, color.uGreen());
    EXPECT_EQ(blue, color.uBlue());
    EXPECT_EQ(alpha, color.uAlpha());
}

TEST_F(FkRenderEngineTest, Render2Buffer) {
    auto src = engine->addMaterial();
    EXPECT_EQ(src->isInvalid(), false);
    FkSize size(32, 32);
    FkIntVec2 pos(size.getWidth() / 2, size.getHeight() / 2);
    // Test white
    auto white = FkColor::white();
    EXPECT_EQ(engine->updateMaterial(src, size, white), FK_OK);
    testColor(engine, src, size, pos, white);
    // Test black
    size = FkSize(128, 128);
    auto black = FkColor::black();
    EXPECT_EQ(engine->updateMaterial(src, size, black), FK_OK);
    testColor(engine, src, size, pos, black);
    EXPECT_EQ(engine->removeMaterial(src), FK_OK);
}

TEST_F(FkRenderEngineTest, RenderLayer) {
    auto whiteMaterial = engine->addMaterial();
    EXPECT_EQ(whiteMaterial->isInvalid(), false);
    FkSize size(32, 32);
    auto white = FkColor::white();
    EXPECT_EQ(engine->updateMaterial(whiteMaterial, size, white), FK_OK);

    auto blackMaterial = engine->addMaterial();
    EXPECT_EQ(blackMaterial->isInvalid(), false);
    auto black = FkColor::black();
    EXPECT_EQ(engine->updateMaterial(blackMaterial, size, black), FK_OK);

    std::shared_ptr<FkDeviceEntity> device = std::make_shared<FkTexDeviceEntity>(blackMaterial);
    auto materials = makeMaterials(whiteMaterial, size, FkIntVec2(size.getWidth() / 2, 0));
    EXPECT_EQ(engine->renderDevice(materials, device), FK_OK);
    FkIntVec2 pos(0, 0);
    testColor(engine, blackMaterial, size, pos, black);
    pos = FkIntVec2(size.getWidth(), 0);
    testColor(engine, blackMaterial, size, pos, white);
    EXPECT_EQ(engine->removeMaterial(blackMaterial), FK_OK);
    EXPECT_EQ(engine->removeMaterial(whiteMaterial), FK_OK);
}