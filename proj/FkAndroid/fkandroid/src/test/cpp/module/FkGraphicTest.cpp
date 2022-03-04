/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "gtest/gtest.h"
#include "FkGraphicContext.h"
#include "FkGraphicTexture.h"
#include "GLES2/gl2.h"
#include "FkGraphicLayer.h"
#include "FkTexIDComponent.h"
#include "FkScaleComponent.h"
#include "FkGraphicFrameObject.h"
#include "FkGraphicProgram.h"

TEST(FkGraphicEntityTest, Context) {
    auto layer = std::make_shared<FkGraphicLayer>();
    auto tex = std::make_shared<FkTexIDComponent>();
    layer->addComponent(tex);

    std::vector<std::shared_ptr<FkComponent>> vec;
    EXPECT_EQ(layer->findComponents(vec, FkClassType::type<FkTexIDComponent >()), FK_OK);
    vec.clear();
    EXPECT_NE(layer->findComponents(vec, FkClassType::type<FkScaleComponent>()), FK_OK);
}

TEST(FkGraphicTest, Context) {
    auto context = std::make_shared<FkGraphicContext>("Test");
    EXPECT_EQ(context->create(), FK_OK);
    context->destroy();
}

TEST(FkGraphicTest, Alloc) {
    auto context = std::make_shared<FkGraphicContext>("Test");
    EXPECT_EQ(context->create(), FK_OK);
    EXPECT_EQ(context->makeCurrent(), FK_OK);

    auto allocator = std::make_shared<FkGraphicAllocator>();
    {
        FkTexDescription desc(GL_TEXTURE_2D);
        desc.fmt = FkColor::kFormat::RGBA;
        auto tex = allocator->alloc(desc);
        EXPECT_NE(tex, nullptr);
        tex->update(FkColor::kFormat::RGBA, 16, 16);
        EXPECT_NE(allocator->size(), 0);
    }
    EXPECT_EQ(allocator->size(), 0);
    EXPECT_NE(allocator->capacity(), 0);
    allocator->release();
    EXPECT_EQ(allocator->size(), 0);
    EXPECT_EQ(allocator->capacity(), 0);

    context->destroy();
    EXPECT_EQ(eglGetCurrentContext(), EGL_NO_CONTEXT);
}

TEST(FkGraphicFBOTest, Alloc) {
    auto context = std::make_shared<FkGraphicContext>("Test");
    EXPECT_EQ(context->create(), FK_OK);
    EXPECT_EQ(context->makeCurrent(), FK_OK);

    auto allocator = std::make_shared<FkGraphicFBOAllocator>();
    {
        int32_t desc = 0;
        auto fbo = allocator->alloc(desc);
        EXPECT_NE(fbo, nullptr);
        EXPECT_NE(allocator->size(), 0);
    }
    EXPECT_EQ(allocator->size(), 0);
    EXPECT_NE(allocator->capacity(), 0);
    allocator->release();
    EXPECT_EQ(allocator->size(), 0);
    EXPECT_EQ(allocator->capacity(), 0);

    context->destroy();
    EXPECT_EQ(eglGetCurrentContext(), EGL_NO_CONTEXT);
}

TEST(FkGraphicProgramTest, Alloc) {
    auto context = std::make_shared<FkGraphicContext>("Test");
    EXPECT_EQ(context->create(), FK_OK);
    EXPECT_EQ(context->makeCurrent(), FK_OK);

    auto allocator = std::make_shared<FkGraphicProgramAllocator>();
    {
        FkProgramDescription desc(FkProgramDescription::kType::MATRIX);
        auto program = allocator->alloc(desc);
        EXPECT_NE(program, nullptr);
        EXPECT_NE(allocator->size(), 0);
    }
    EXPECT_EQ(allocator->size(), 0);
    EXPECT_NE(allocator->capacity(), 0);
    allocator->release();
    EXPECT_EQ(allocator->size(), 0);
    EXPECT_EQ(allocator->capacity(), 0);

    context->destroy();
    EXPECT_EQ(eglGetCurrentContext(), EGL_NO_CONTEXT);
}