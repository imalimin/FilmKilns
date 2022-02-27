/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "gtest/gtest.h"
#include "FkRenderEngine.h"
#include "FkTestDefine.h"
#include "FkFuncCompo.h"
#include "FkBitmap.h"

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
    auto material = std::make_shared<FkMaterialCompo>(FK_ID_NONE);
    auto dst = std::make_shared<FkMaterialCompo>(FK_ID_NONE);
    auto device = std::make_shared<FkDeviceEntity>(dst);
    EXPECT_NE(engine->render(material, device), FK_OK);
    EXPECT_EQ(engine->create(), FK_OK);
    device = std::make_shared<FkDeviceEntity>(dst);
    EXPECT_NE(engine->render(material, device), FK_OK);
    EXPECT_EQ(engine->start(), FK_OK);
    device = std::make_shared<FkDeviceEntity>(dst);
    EXPECT_EQ(engine->render(material, device), FK_OK);
    EXPECT_EQ(engine->stop(), FK_OK);
    device = std::make_shared<FkDeviceEntity>(dst);
    EXPECT_NE(engine->render(material, device), FK_OK);
    EXPECT_EQ(engine->destroy(), FK_OK);
}

TEST(FkRenderTest, NewMaterial) {
    FK_NEW_INSTANCE(engine, FkRenderEngine, "RenderEngine")
    auto src = engine->newMaterial();
    EXPECT_EQ(src->isUseless(), false);
    FK_DELETE_INSTANCE(engine)
}

static void testColor(std::shared_ptr<FkRenderEngine> &engine,
                      std::shared_ptr<FkMaterialCompo> &src,
                      FkSize &size,
                      FkColor &color) {
    int width = size.getWidth();
    int height = size.getHeight();

    auto buf = std::make_shared<FkBuffer>(width * height * 4);
    memset(buf->data(), 125, buf->capacity());
    auto promise = std::make_shared<std::promise<int>>();
    std::shared_ptr<FkDeviceEntity> device = std::make_shared<FkBufDeviceEntity>(buf);
    device->addComponent(std::make_shared<FkFuncCompo>([promise, buf, &size]() {
//        std::string path = "/storage/emulated/0/Android/data/com.alimin.fk.test/cache/000000.bmp";
//        FkBitmap::write(path, buf->data(), buf->capacity(), size.getWidth(), size.getHeight());
        promise->set_value(FK_OK);
    }));
    EXPECT_EQ(engine->render(src, device), FK_OK);
    EXPECT_EQ(promise->get_future().get(), FK_OK);
    // Center
    int pos = height / 2 * width * 4 + width / 2 * 4;
    auto red = buf->data()[pos + 0];
    auto green = buf->data()[pos + 1];
    auto blue = buf->data()[pos + 2];
    auto alpha = buf->data()[pos + 3];
    EXPECT_EQ(red, color.red);
    EXPECT_EQ(green, color.green);
    EXPECT_EQ(blue, color.blue);
    EXPECT_EQ(alpha, color.alpha);
}


TEST(FkRenderTest, Render2Buffer) {
    FK_NEW_INSTANCE(engine, FkRenderEngine, "RenderEngine")
    auto src = engine->newMaterial();
    EXPECT_EQ(src->isUseless(), false);
    FkSize size(32, 32);
    // Test white
    auto white = FkColor::white();
    EXPECT_EQ(engine->updateMaterial(src, size, white), FK_OK);
    testColor(engine, src, size, white);
    // Test black
    size = FkSize(128, 128);
    auto black = FkColor::black();
    EXPECT_EQ(engine->updateMaterial(src, size, black), FK_OK);
    testColor(engine, src, size, black);
    FK_DELETE_INSTANCE(engine)
}

TEST(FkRenderTest, RenderLayer) {
    FK_NEW_INSTANCE(engine, FkRenderEngine, "RenderEngine")
    auto src0 = engine->newMaterial();
    EXPECT_EQ(src0->isUseless(), false);
    FkSize size(32, 32);
    auto white = FkColor::white();
    EXPECT_EQ(engine->updateMaterial(src0, size, white), FK_OK);

    auto src1 = engine->newMaterial();
    EXPECT_EQ(src1->isUseless(), false);
    auto black = FkColor::black();
    EXPECT_EQ(engine->updateMaterial(src1, size, black), FK_OK);

    std::shared_ptr<FkDeviceEntity> device = std::make_shared<FkTexDeviceEntity>(src1);
    EXPECT_EQ(engine->render(src0, device), FK_OK);
    testColor(engine, src1, size, white);
    FK_DELETE_INSTANCE(engine)
}
