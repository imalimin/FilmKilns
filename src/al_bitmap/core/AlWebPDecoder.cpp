/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "AlWebPDecoder.h"
#include "Logcat.h"

#define TAG "AlWebpDecoder"

AlWebPDecoder::AlWebPDecoder(std::string path) : AlAbsDecoder(), path(path) {
}

AlBitmapInfo AlWebPDecoder::getInfo() {
    AlBitmapInfo info;
    AlBuffer *fBuf = nullptr;
    if (!_readFile(&fBuf)) {
        Logcat::i(TAG, "%s(%d): failed", __FUNCTION__, __LINE__);
        return info;
    }
    WebPBitstreamFeatures features;
    memset(&features, 0, sizeof(WebPBitstreamFeatures));
    int ret = WebPGetFeatures(fBuf->data(), fBuf->size(), &features);
    if (VP8_STATUS_OK != ret) {
        Logcat::i(TAG, "%s(%d): failed", __FUNCTION__, __LINE__);
        return info;
    }
    Logcat::i(TAG, "%dx%d, has_alpha=%d, has_animation=%d", features.width, features.height,
              features.has_alpha, features.has_animation);
    info.width = features.width;
    info.height = features.height;
    info.depth = 8;
    info.colorSpace = AlColorSpace::RGBA;
    return info;
}

HwResult AlWebPDecoder::process(AlBuffer **buf, AlBitmapInfo *info) {
    AlBuffer *fBuf = nullptr;
    if (!_readFile(&fBuf)) {
        Logcat::i(TAG, "%s(%d): failed", __FUNCTION__, __LINE__);
        return Hw::FAILED;
    }
    *buf = AlBuffer::alloc(info->width * info->height * 4);
    uint8_t *data = WebPDecodeRGBAInto(fBuf->data(), fBuf->size(),
                                       (*buf)->data(), (*buf)->size(),
                                       info->width * 4);
    if (nullptr == data) {
        Logcat::i(TAG, "%s(%d): failed", __FUNCTION__, __LINE__);
        return Hw::FAILED;
    }
    return Hw::SUCCESS;
}

bool AlWebPDecoder::_readFile(AlBuffer **buf) {
    *buf = nullptr;
    size_t size;
    FILE *in = fopen(path.c_str(), "rb");
    if (nullptr == in) {
        return false;
    }
    fseek(in, 0, SEEK_END);
    size = ftell(in);
    fseek(in, 0, SEEK_SET);
    *buf = AlBuffer::alloc(size);

    bool ok = (fread((*buf)->data(), size, 1, in) == 1);
    fclose(in);
    if (!ok) {
        delete *buf;
        *buf = nullptr;
        return false;
    }
    return true;
}
