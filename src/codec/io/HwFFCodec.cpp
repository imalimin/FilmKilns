/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwFFCodec.h"
#include "Logcat.h"

HwFFCodec::HwFFCodec(int32_t codecId) : HwAbsCodec(codecId) {

}

HwFFCodec::~HwFFCodec() {

}

void HwFFCodec::release() {

}

HwResult HwFFCodec::start() {
    if (codecId <= AV_CODEC_ID_NONE) {
        return Hw::FAILED;
    }
    AVCodecID id = static_cast<AVCodecID>(codecId);
    AVCodec *pCodec = avcodec_find_encoder(id);
    if (!pCodec) {
        release();
        Logcat::e("HWVC", "HwFFCodec::initialize could not find %d codec!", id);
        return false;
    }
    ctx = avcodec_alloc_context3(pCodec);
    if (!ctx) {
        release();
        Logcat::e("HWVC", "HwFFCodec::initialize could not init %d codec context!", id);
        return false;
    }
    switch (id) {
        case AV_CODEC_ID_H264: {
            break;
        }
        case AV_CODEC_ID_AAC_LATM:
        case AV_CODEC_ID_AAC: {
            break;
        }
        default:
            return Hw::FAILED;
    }
    return Hw::SUCCESS;
}