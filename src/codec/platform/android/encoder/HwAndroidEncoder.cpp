/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "HwAndroidEncoder.h"
#include "Logcat.h"

HwAndroidEncoder::HwAndroidEncoder() : HwAbsEncoder() {

}

HwAndroidEncoder::~HwAndroidEncoder() {

}

bool HwAndroidEncoder::prepare(string path, int width, int height) {
    this->width = width;
    this->height = height;
    return configure();
}

HwResult HwAndroidEncoder::write(HwAbsMediaFrame *frame) {
    return HwResult(0);
}

bool HwAndroidEncoder::stop() {
    return false;
}

void HwAndroidEncoder::release() {

}

bool HwAndroidEncoder::configure() {
    AMediaFormat *format = AMediaFormat_new();
    AMediaFormat_setString(format, AMEDIAFORMAT_KEY_MIME, "video/avc");
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_WIDTH, width);
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_HEIGHT, height);
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_COLOR_FORMAT, 19);
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_BIT_RATE, width * height * 3);
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_FRAME_RATE, 30);
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_I_FRAME_INTERVAL, 10);
    const char *mime;
    AMediaFormat_getString(format, AMEDIAFORMAT_KEY_MIME, &mime);
    codec = AMediaCodec_createDecoderByType(mime);
    if (!codec) {
        Logcat::e("HWVC", "HwAndroidEncoder codec alloc failed");
        AMediaFormat_delete(format);
        return false;
    }
    media_status_t ret = AMediaCodec_configure(codec, format, nullptr, nullptr, AMEDIACODEC_CONFIGURE_FLAG_ENCODE);
    if (media_status_t::AMEDIA_OK != ret) {
        Logcat::e("HWVC", "HwAndroidEncoder codec configure failed(%d)", ret);
        AMediaFormat_delete(format);
        return false;
    }
    ret = AMediaCodec_start(codec);
    if (media_status_t::AMEDIA_OK != ret) {
        Logcat::e("HWVC", "HwAndroidEncoder codec start failed(%d)", ret);
        AMediaFormat_delete(format);
        return false;
    }
    AMediaFormat_delete(format);
    return true;
}
