/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/AlEncoderBuilder.h"
#include "../include/HwAsyncEncoder.h"
#include "../include/HwFFmpegEncoder.h"
#include "../platform/android/encoder/HwAndroidEncoder.h"

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

AlEncoderBuilder &AlEncoderBuilder::setEnableAsyn(bool enable) {
    this->enableAsyn = enable;
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

HwAbsVideoEncoder *AlEncoderBuilder::build() {
    if (0 != size.width % 16 || 0 != size.height % 16) {
        AlLogE(TAG, "Not align 16. %dx%d", size.width, size.height);
        return nullptr;
    }
    HwAbsVideoEncoder *encoder = nullptr;
    if (enableAsyn) {
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
           "Alloc encoder video(width=%d, height=%d, bitrate=%d, profile=%s), audio(fmt=%d, sample rate=%d, channels=%d), out=%s",
           size.width, size.height, bitrate, profile.c_str(),
           (int) audioFormat.getFormat(), audioFormat.getSampleRate(), audioFormat.getChannels(),
           output.c_str());
    encoder->setBitrate(bitrate);
    encoder->setProfile(profile);
    if (!encoder->prepare(output, size.width, size.height, audioFormat)) {
        AlLogE(TAG, "Prepare video encoder failed");
        delete encoder;
        return nullptr;
    }
    return encoder;
}
