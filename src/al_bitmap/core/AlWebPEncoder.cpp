/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "AlWebPEncoder.h"
#include "Logcat.h"

#define TAG "AlWebPEncoder"

AlWebPEncoder::AlWebPEncoder(std::string path) : AlAbsEncoder() {

}

AlWebPEncoder::~AlWebPEncoder() {

}

HwResult AlWebPEncoder::process(AlBuffer *buf, AlBitmapInfo *info, std::string path) {
    uint8_t *oBuf = nullptr;
    size_t size = WebPEncodeRGBA(buf->data(), info->width, info->height,
                                 info->width * 4, 80, &oBuf);
    if (0 == size || nullptr == oBuf) {
        Logcat::i(TAG, "%s(%d): failed", __FUNCTION__, __LINE__);
        return Hw::FAILED;
    }
    FILE *out = fopen(path.c_str(), "wb");
    if (nullptr == out) {
        Logcat::i(TAG, "%s(%d): failed", __FUNCTION__, __LINE__);
        return Hw::FAILED;
    }
    fwrite(oBuf, size, 1, out);
    fclose(out);
    return Hw::SUCCESS;
}
