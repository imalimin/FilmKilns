/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwAbsCodec.h"

const string HwAbsCodec::KEY_MIME = "media-mime";
const string HwAbsCodec::KEY_FORMAT = "media-format";
const string HwAbsCodec::KEY_PROFILE = "media-profile";
const string HwAbsCodec::KEY_LEVEL = "media-level";
const string HwAbsCodec::KEY_BIT_RATE = "media-bit-rate";
const string HwAbsCodec::KEY_WIDTH = "video-width";
const string HwAbsCodec::KEY_HEIGHT = "video-height";
const string HwAbsCodec::KEY_FPS = "video-fps";
const string HwAbsCodec::KEY_QUALITY = "video-quality";
const string HwAbsCodec::KEY_SAMPLE_RATE = "audio-sample-rate";
const string HwAbsCodec::KEY_CHANNELS = "audio-channels";
const string HwAbsCodec::KEY_FRAME_SIZE = "audio-frame-size";
const string HwAbsCodec::KEY_CSD_0 = "csd-0";
const string HwAbsCodec::KEY_CSD_1 = "csd-1";
const string HwAbsCodec::KEY_CSD_2 = "csd-2";
const string HwAbsCodec::KEY_CSD_3 = "csd-3";

HwAbsCodec::HwAbsCodec(int32_t codecId) : Object(), codecId(codecId) {

}

HwAbsCodec::~HwAbsCodec() {

}

HwResult HwAbsCodec::configure(HwBundle &format) {
    this->format = format;
    return Hw::SUCCESS;
}

int32_t HwAbsCodec::getCodecId() { return codecId; }

HwBundle &HwAbsCodec::getFormat() { return format; }