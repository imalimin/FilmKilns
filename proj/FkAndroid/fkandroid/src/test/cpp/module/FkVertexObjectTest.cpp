//
// Created by Alimin on 2021/8/5.
//

#include "gtest/gtest.h"
#include "FkVertexObject.h"

TEST(FkVertexObjectTest, Context) {
    auto allocator = std::make_shared<FkVertexObjectAllocator>();

    FkVBODescription desc(sizeof(float) * 2 * 2);
    auto vbo0 = allocator->alloc(desc);
    EXPECT_NE(vbo0, nullptr);
}