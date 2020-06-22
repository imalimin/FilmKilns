//
// Created by mingyi.li on 2019/6/16.
//

#include "../include/HwSampleFormat.h"
#include "../include/HwAbsMediaFrame.h"

const HwSampleFormat HwSampleFormat::NONE = HwSampleFormat(HwFrameFormat::HW_FMT_NONE, 0, 0);

HwSampleFormat::HwSampleFormat(HwFrameFormat format,
                               uint16_t channels,
                               uint32_t sampleRate)
        : Object() {
    this->format = format;
    this->channels = channels;
    this->sampleRate = sampleRate;
}

HwSampleFormat::HwSampleFormat(const HwSampleFormat &o) : Object() {
    this->format = o.format;
    this->channels = o.channels;
    this->sampleRate = o.sampleRate;
}

HwSampleFormat::~HwSampleFormat() {
    this->format = HwFrameFormat::HW_FMT_NONE;
    this->channels = 0;
    this->sampleRate = 0;
}

HwFrameFormat HwSampleFormat::getFormat() { return format; }

int32_t HwSampleFormat::getAVFormat() {
    return HwAbsMediaFrame::convertAudioFrameFormat(getFormat());
}

uint16_t HwSampleFormat::getChannels() { return channels; }

uint32_t HwSampleFormat::getSampleRate() { return sampleRate; }

HwSampleFormat &HwSampleFormat::operator=(const HwSampleFormat &o) {
    this->format = o.format;
    this->channels = o.channels;
    this->sampleRate = o.sampleRate;
    return *this;
}

bool HwSampleFormat::operator==(const HwSampleFormat &o) {
    return this->format == o.format &&
           this->channels == o.channels &&
           this->sampleRate == o.sampleRate;
}

size_t HwSampleFormat::getBytesPerSample() {
    return static_cast<size_t>(HwAbsMediaFrame::getBytesPerSample(format) * channels);
}

bool HwSampleFormat::valid() {
    bool validChannel = 1 == channels || 2 == channels;
    bool validSampleRate = 8000 == sampleRate || 16000 == sampleRate || 32000 == sampleRate ||
                           44100 == sampleRate || 48000 == sampleRate;
    return HwFrameFormat::HW_FMT_NONE != format && validChannel && validSampleRate;
}
