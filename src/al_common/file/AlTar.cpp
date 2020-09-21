/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlTar.h"
#include "Logcat.h"

#define TAG "AlTar"

static bool isZero(AlBuffer *buf) {
    size_t size = buf->size();
    for (int i = 0; i < size; ++i) {
        if (0 != buf->data()[i]) {
            return false;
        }
    }
    return true;
}

AlTar *AlTar::unpack(std::string tar, std::string destDir) {
    FILE *in = fopen(tar.c_str(), "rb");
    if (nullptr == in) {
        Logcat::i(TAG, "%s(%d)", __FUNCTION__, __LINE__);
        return nullptr;
    }
//    unsigned int offset = 0;
    int32_t count = 0;
    bool update = true;
    while (true) {
        AlArchive *archive = new AlArchive();
        size_t size = archive->block->size();
        if (update && size != fread(archive->block->data(), archive->block->size(), 1, in)) {
            Logcat::i(TAG, "%s(%d)", __FUNCTION__, __LINE__);
            delete archive;
            break;
        }
        update = true;
        if (isZero(archive->block)) {
            if (size != fread(archive->block->data(), archive->block->size(), 1, in)) {
                Logcat::i(TAG, "%s(%d)", __FUNCTION__, __LINE__);
                delete archive;
                break;
            }
            if (size != archive->block->size()) {
                Logcat::i(TAG, "%s(%d)", __FUNCTION__, __LINE__);
                delete archive;
                break;
            }
            update = false;
        }
        ++count;
    }
    fclose(in);
    return nullptr;
}

AlTar::AlTar() {

}

AlTar::~AlTar() {

}

AlTar::AlArchive::AlArchive() {
    block = AlBuffer::alloc(512);
}

AlTar::AlArchive::~AlArchive() {
    delete block;
    block = nullptr;
}
