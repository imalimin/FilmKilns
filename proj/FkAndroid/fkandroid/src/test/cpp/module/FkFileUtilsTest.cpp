/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "gtest/gtest.h"
#include "FkFileUtils.h"
#include "FkString.h"
#include "FkTimeUtils.h"

#define FK_ANDROID_TEST_CACHE_DIR "/storage/emulated/0/Android/data/com.alimin.fk.test/cache"

class FkFileUtilsTest : public testing::Test {
    void SetUp() override {
    }

    void TearDown() override {
    }

protected:
};

TEST_F(FkFileUtilsTest, copy) {
    std::string dir = FkString(FK_ANDROID_TEST_CACHE_DIR)
            .append("/images")
            .toString();
    std::string src = dir + "/image_0.jpg";
    std::string dst = dir + "/image_1.jpg";
    EXPECT_EQ(FkFileUtils::copy(src, dst), FK_OK);
    FILE *file = fopen(dst.c_str(), "rb");
    EXPECT_NE(file, nullptr);
    if (file) {
        fclose(file);
    }
}

TEST_F(FkFileUtilsTest, mkdir) {
    std::string dir = FkString(FK_ANDROID_TEST_CACHE_DIR)
            .append("/mkdirs/")
            .append(FkTimeUtils::getCurrentTimeUS())
            .toString();
    EXPECT_EQ(FkFileUtils::mkdirs(dir), FK_OK);
    EXPECT_EQ(FkFileUtils::exist(dir), true);
}

TEST_F(FkFileUtilsTest, mkdirs) {
    std::string dir = FkString(FK_ANDROID_TEST_CACHE_DIR)
            .append("/mkdirs/")
            .append(FkTimeUtils::getCurrentTimeUS())
            .append("/")
            .append(FkTimeUtils::getCurrentTimeUS())
            .toString();
    EXPECT_EQ(FkFileUtils::mkdirs(dir), FK_OK);
    EXPECT_EQ(FkFileUtils::exist(dir), true);
}

TEST_F(FkFileUtilsTest, exist) {
    std::string dir = FK_ANDROID_TEST_CACHE_DIR;
    EXPECT_EQ(FkFileUtils::exist(dir), true);
    dir = FkString(FK_ANDROID_TEST_CACHE_DIR)
            .append("/")
            .append(FkTimeUtils::getCurrentTimeUS())
            .toString();
    EXPECT_EQ(FkFileUtils::exist(dir), false);
}

TEST_F(FkFileUtilsTest, parent) {
    std::string dir = FK_ANDROID_TEST_CACHE_DIR;
    EXPECT_EQ(FkFileUtils::exist(dir), true);
    dir = FkString(FK_ANDROID_TEST_CACHE_DIR)
            .append("/")
            .append(FkTimeUtils::getCurrentTimeUS())
            .toString();
    EXPECT_EQ(FkFileUtils::parent(dir), FK_ANDROID_TEST_CACHE_DIR);
}

TEST_F(FkFileUtilsTest, Trim) {
    std::string dir = "/sdcard/dir/";
    std::string expect = "/sdcard/dir";
    FkFileUtils::trim(dir);
    EXPECT_EQ(dir, expect);
}
