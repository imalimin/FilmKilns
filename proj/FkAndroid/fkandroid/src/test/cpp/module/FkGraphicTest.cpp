/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "gtest/gtest.h"
#include "FkContextCompo.h"
#include "FkGraphicTexture.h"
#include "GLES2/gl2.h"
#include "FkGraphicLayer.h"
#include "FkScaleComponent.h"
#include "FkGraphicFrameObject.h"
#include "FkGraphicProgram.h"
#include "FkGLDefinition.h"

class FkGraphicContextTest : public testing::Test {
    void SetUp() override {
        context = std::make_shared<FkContextCompo>(FK_GL_VER_2, "Test");
        EXPECT_EQ(context->create(), FK_OK);
        EXPECT_EQ(context->makeCurrent(), FK_OK);
        EXPECT_NE(eglGetCurrentContext(), nullptr);
    }

    void TearDown() override {
        context->destroy();
        EXPECT_EQ(eglGetCurrentContext(), nullptr);
        context = nullptr;
    }

protected:
    std::shared_ptr<FkContextCompo> context = nullptr;
};

TEST_F(FkGraphicContextTest, AllocTex) {
    auto allocator = std::make_shared<FkGraphicAllocator>(1080 * 1920 * 4 * 10);
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
}

TEST_F(FkGraphicContextTest, AllocFbo) {
    auto allocator = std::make_shared<FkGraphicFBOAllocator>(10);
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
}

TEST_F(FkGraphicContextTest, AllocProgram) {
    auto allocator = std::make_shared<FkGraphicProgramAllocator>(10);
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
}