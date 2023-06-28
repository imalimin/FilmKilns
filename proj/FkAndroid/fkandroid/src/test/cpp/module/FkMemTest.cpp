/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "gtest/gtest.h"
#include "FkMemUtils.h"
#include "FkBitmap.h"

#define FK_ANDROID_TEST_CACHE_IMAGE_0 "/storage/emulated/0/Android/data/com.alimin.fk.test/cache/images/image_0.jpg"
#define FK_ANDROID_TEST_CACHE_IMAGE_SUB_0 "/storage/emulated/0/Android/data/com.alimin.fk.test/cache/images/image_sub_0.jpg"
#define FK_ANDROID_TEST_CACHE_IMAGE_SUB_1 "/storage/emulated/0/Android/data/com.alimin.fk.test/cache/images/image_sub_1.jpg"
#define FK_ANDROID_TEST_CACHE_IMAGE_SUB_2 "/storage/emulated/0/Android/data/com.alimin.fk.test/cache/images/image_sub_2.jpg"
#define FK_ANDROID_TEST_CACHE_IMAGE_SUB_3 "/storage/emulated/0/Android/data/com.alimin.fk.test/cache/images/image_sub_3.jpg"

TEST(FkMemUtils, CopySubImage) {
    std::string path = FK_ANDROID_TEST_CACHE_IMAGE_0;
    auto bmp = FkBitmap::from(path);
    FkSize size(bmp->getWidth() / 2, bmp->getHeight() / 2);
    auto src = FkBuffer::wrap(bmp->getPixels(), bmp->getByteSize());
    auto dst = FkBuffer::alloc(size.getWidth() * size.getHeight() * 4);
    FkIntVec2 pos(0, 0);
    FkMemUtils::copySubImage(src, bmp->getWidth(), dst, size, pos);
    auto ret = FkBitmap::write(FK_ANDROID_TEST_CACHE_IMAGE_SUB_0,
                               FkImage::Format::kJPEG, dst, size, 80);
    EXPECT_EQ(ret, FK_OK);
    pos = FkIntVec2(size.getWidth(), 0);
    FkMemUtils::copySubImage(src, bmp->getWidth(), dst, size, pos);
    ret = FkBitmap::write(FK_ANDROID_TEST_CACHE_IMAGE_SUB_1,
                          FkImage::Format::kJPEG, dst, size, 80);
    EXPECT_EQ(ret, FK_OK);
    pos = FkIntVec2(0, size.getHeight());
    FkMemUtils::copySubImage(src, bmp->getWidth(), dst, size, pos);
    ret = FkBitmap::write(FK_ANDROID_TEST_CACHE_IMAGE_SUB_2,
                          FkImage::Format::kJPEG, dst, size, 80);
    EXPECT_EQ(ret, FK_OK);
    pos = FkIntVec2(size.getWidth(), size.getHeight());
    FkMemUtils::copySubImage(src, bmp->getWidth(), dst, size, pos);
    ret = FkBitmap::write(FK_ANDROID_TEST_CACHE_IMAGE_SUB_3,
                          FkImage::Format::kJPEG, dst, size, 80);
    EXPECT_EQ(ret, FK_OK);
}