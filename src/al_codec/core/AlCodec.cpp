/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlCodec.h"

const string AlCodec::KEY_MIME = "media-mime";
const string AlCodec::KEY_FORMAT = "media-format";
const string AlCodec::KEY_PROFILE = "media-profile";
const string AlCodec::KEY_PRESET = "media-preset";
const string AlCodec::KEY_LEVEL = "media-level";
const string AlCodec::KEY_BIT_RATE = "media-bit-rate";
const string AlCodec::KEY_WIDTH = "video-width";
const string AlCodec::KEY_HEIGHT = "video-height";
const string AlCodec::KEY_FPS = "video-fps";
const string AlCodec::KEY_QUALITY = "video-quality";
const string AlCodec::KEY_SAMPLE_RATE = "audio-sample-rate";
const string AlCodec::KEY_CHANNELS = "audio-channels";
const string AlCodec::KEY_FRAME_SIZE = "audio-frame-size";
const string AlCodec::KEY_CSD_0 = "csd-0";
const string AlCodec::KEY_CSD_1 = "csd-1";
const string AlCodec::KEY_CSD_2 = "csd-2";
const string AlCodec::KEY_CSD_3 = "csd-3";

AlCodec::Desc::Desc() : Object(), id(kID::NONE), type(kType::SOFT) {

}

AlCodec::Desc::Desc(AlCodec::kID id, AlCodec::kType type) : Object(), id(id), type(type) {

}

AlCodec::Desc::Desc(const Desc &o) : Object(), id(o.id), type(o.type) {

}

AlCodec::Desc::~Desc() {

}

AlCodec::AlCodec(AlCodec::kID id) : Object(), id(id) {

}

AlCodec::~AlCodec() {

}

HwResult AlCodec::configure(HwBundle &format) {
    this->format = format;
    return Hw::SUCCESS;
}

AlCodec::kID AlCodec::getCodecID() { return id; }

HwBundle &AlCodec::getFormat() { return format; }

AlCodec::kMediaType AlCodec::getMediaType() {
    switch (getCodecID()) {
        case AlCodec::kID::H264: {
            return AlCodec::kMediaType::VIDEO;
        }
        case AlCodec::kID::AAC: {
            return AlCodec::kMediaType::AUDIO;
        }
        default: {
            return AlCodec::kMediaType::UNKNOWN;
        }
    }
}

