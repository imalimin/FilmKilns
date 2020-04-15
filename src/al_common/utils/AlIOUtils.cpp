/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "AlIOUtils.h"
#include "StringUtils.h"
#include "Logcat.h"

#define TAG "AlIOUtils"

HwResult AlIOUtils::readFile(std::string *inFile, AlBuffer **outBuf) {
    if (nullptr == outBuf || StringUtils::isEmpty(inFile)) {
        Logcat::i(TAG, "%s(%d) failed", __FUNCTION__, __LINE__);
        return Hw::FAILED;
    }
    *outBuf = nullptr;
    size_t size;
    FILE *in = fopen(inFile->c_str(), "rb");
    if (nullptr == in) {
        Logcat::i(TAG, "%s(%d) failed", __FUNCTION__, __LINE__);
        return Hw::FAILED;
    }
    fseek(in, 0, SEEK_END);
    size = ftell(in);
    fseek(in, 0, SEEK_SET);
    *outBuf = AlBuffer::alloc(size);

    bool ok = (fread((*outBuf)->data(), size, 1, in) == 1);
    fclose(in);
    if (!ok) {
        delete *outBuf;
        *outBuf = nullptr;
        Logcat::i(TAG, "%s(%d) failed", __FUNCTION__, __LINE__);
        return Hw::FAILED;
    }
    return Hw::SUCCESS;
}
