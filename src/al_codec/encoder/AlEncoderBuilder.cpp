/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlEncoderBuilder.h"
#include "HwAsyncEncoder.h"
#include "HwFFmpegEncoder.h"
#include "platform/android/encoder/HwAndroidEncoder.h"
#include "platform/android/core/AlAndroidCodecCompat.h"

#define TAG "AlEncoderBuilder"

AlEncoderBuilder::AlEncoderBuilder()
        : Object(), size(0, 0), audioFormat(HwFrameFormat::HW_FMT_NONE, 0, 0) {

}

AlEncoderBuilder::~AlEncoderBuilder() {

}

AlEncoderBuilder &AlEncoderBuilder::setSize(AlSize size) {
    this->size = size;
    return *this;
}

AlEncoderBuilder &AlEncoderBuilder::setAudioFormat(HwSampleFormat &format) {
    this->audioFormat = format;
    return *this;
}

AlEncoderBuilder &AlEncoderBuilder::setEnableHardware(bool enable) {
    this->enableHardware = enable;
    return *this;
}

AlEncoderBuilder &AlEncoderBuilder::setEnableAsync(bool enable) {
    this->enableAsync = enable;
    return *this;
}

AlEncoderBuilder &AlEncoderBuilder::setBitrate(int32_t rate) {
    this->bitrate = rate;
    return *this;
}

AlEncoderBuilder &AlEncoderBuilder::setProfile(std::string profile) {
    this->profile = profile;
    return *this;
}

AlEncoderBuilder &AlEncoderBuilder::setPreset(std::string preset) {
    this->preset = preset;
    return *this;
}

AlEncoderBuilder &AlEncoderBuilder::setOutput(std::string output) {
    this->output = output;
    return *this;
}

static void showEncoderInfo() {
    std::string msg;
    AVInputFormat *aif = av_iformat_next(nullptr);
    msg.append("ENCODERS: ");
    while (aif != nullptr) {
        msg.append(aif->name);
        msg.append(", ");
        aif = aif->next;
    }
    AlLogI(TAG, "%s", msg.c_str());
}

static void showDecoderInfo() {
    std::string msg;
    msg.append("DECODERS: ");
    AVOutputFormat *aof = av_oformat_next(nullptr);
    while (aof != nullptr) {
        msg.append(aof->name);
        msg.append(", ");
        aof = aof->next;
    }
    AlLogI(TAG, "%s", msg.c_str());
}

static void showFFInfo() {
    av_register_all();
    AlLogI(TAG, "FFmpeg ver %d.%d.%d",
           LIBSWRESAMPLE_VERSION_MAJOR,
           LIBSWRESAMPLE_VERSION_MINOR,
           LIBSWRESAMPLE_VERSION_MICRO);
    showEncoderInfo();
    showDecoderInfo();
}

HwAbsVideoEncoder *AlEncoderBuilder::build() {
    showFFInfo();
    if (0 != size.width % 16 || 0 != size.height % 16) {
        AlLogE(TAG, "Not align 16. %dx%d", size.width, size.height);
        return nullptr;
    }
    HwAbsVideoEncoder *encoder = nullptr;
    if (enableAsync) {
        if (!enableHardware) {
            encoder = new HwAsyncEncoder();
        } else {
            encoder = new HwAndroidEncoder();
        }
    } else {
        if (!enableHardware) {
            encoder = new HwFFmpegEncoder();
        } else {
            encoder = new HwAndroidEncoder();
        }
    }
    AlLogI(TAG,
           "Alloc encoder video(width=%d, height=%d, bitrate=%d, profile=%s, hardware=%d), audio(fmt=%d, sample rate=%d, channels=%d), out=%s",
           size.width, size.height, bitrate, profile.c_str(), enableHardware,
           (int) audioFormat.getFormat(), audioFormat.getSampleRate(), audioFormat.getChannels(),
           output.c_str());
    encoder->setBitrate(bitrate);
    encoder->setProfile(profile);
    if (!encoder->prepare(output, size.width, size.height, audioFormat)) {
        AlLogE(TAG, "Prepare video encoder failed");
        delete encoder;
        return nullptr;
    }
    auto *codec = new AlAndroidCodecCompat(AlCodec::H264);
    delete codec;
    return encoder;
}
