//
// Created by Alimin on 2021/8/5.
//

#include "FkWithGLContext.h"
#include "FkVertexObject.h"

TEST_F(FkWithGLContext, FkVertexObjectAllocOne) {
    auto allocator = std::make_shared<FkVertexObjectAllocator>();
    FkVBODescription desc(sizeof(float) * 2 * 2);
    auto vbo0 = allocator->alloc(desc);
    EXPECT_NE(vbo0, nullptr);
    EXPECT_EQ(allocator->capacity(), desc.size);
}

TEST_F(FkWithGLContext, FkVertexObjectAllocTwo) {
    auto allocator = std::make_shared<FkVertexObjectAllocator>();
    FkVBODescription desc0(sizeof(float) * 2 * 2);
    auto vbo0 = allocator->alloc(desc0);
    EXPECT_NE(vbo0, nullptr);

    FkVBODescription desc1(sizeof(float) * 2 * 4);
    auto vbo1 = allocator->alloc(desc1);
    EXPECT_NE(vbo1, nullptr);

    EXPECT_EQ(allocator->capacity(), desc0.size + desc1.size);
}

TEST_F(FkWithGLContext, FkVertexObjectAllocTwoAndRecyle) {
    auto allocator = std::make_shared<FkVertexObjectAllocator>();
    FkVBODescription desc0(sizeof(float) * 2 * 2);
    {
        auto vbo0 = allocator->alloc(desc0);
        EXPECT_NE(vbo0, nullptr);
    }

    FkVBODescription desc1(sizeof(float) * 2 * 4);
    auto vbo1 = allocator->alloc(desc1);
    EXPECT_NE(vbo1, nullptr);

    EXPECT_EQ(allocator->capacity(), desc0.size + desc1.size);
    EXPECT_EQ(allocator->size(), desc1.size);
}

TEST_F(FkWithGLContext, FkVertexObjectAllocTwoAndRecyle1) {
    auto allocator = std::make_shared<FkVertexObjectAllocator>();
    FkVBODescription desc0(sizeof(float) * 2 * 2);
    {
        auto vbo0 = allocator->alloc(desc0);
        EXPECT_NE(vbo0, nullptr);
    }

    FkVBODescription desc1(sizeof(float) * 2 * 4);
    auto vbo1 = allocator->alloc(desc1);
    EXPECT_NE(vbo1, nullptr);

    EXPECT_EQ(allocator->capacity(), desc0.size + desc1.size);
    EXPECT_EQ(allocator->size(), desc1.size);

    auto vbo0 = allocator->alloc(desc0);
    EXPECT_NE(vbo0, nullptr);

    EXPECT_EQ(allocator->capacity(), desc0.size + desc1.size);
    EXPECT_EQ(allocator->size(), desc0.size + desc1.size);
}