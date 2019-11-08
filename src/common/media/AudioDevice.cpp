/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/
#include "../include/AudioDevice.h"

AudioDevice::AudioDevice(uint16_t channels,
                         uint32_t sampleRate,
                         uint16_t format,
                         uint32_t samplesPerBuffer) : channels(channels),
                                                      sampleRate(sampleRate),
                                                      format(format),
                                                      samplesPerBuffer(samplesPerBuffer) {
}

uint16_t AudioDevice::getChannels() {
    return channels;
}

uint32_t AudioDevice::getSampleRate() {
    return sampleRate;
}

uint16_t AudioDevice::getFormat() {
    return format;
}

uint32_t AudioDevice::getSamplesPerBuffer() {
    return samplesPerBuffer;
}

uint32_t AudioDevice::getBufferByteSize() {
    uint32_t bufSize = samplesPerBuffer * channels * format;
    bufSize = (bufSize + 7) >> 3;  // bits --> byte
    return bufSize;
}

SLAudioDevice::SLAudioDevice(HwAudioDeviceMode mode,
                             uint16_t channels,
                             uint32_t sampleRate,
                             uint16_t format,
                             uint32_t samplesPerBuffer) : AudioDevice(channels,
                                                                      sampleRate,
                                                                      format,
                                                                      samplesPerBuffer),
                                                          mode(mode) {
}

SLuint32 SLAudioDevice::getChannelMask() {
    switch (channels) {
        case 1:
            return SL_SPEAKER_FRONT_LEFT;
        case 3:
            return SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT | SL_SPEAKER_FRONT_CENTER;
        case 2:
        default:
            return SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT;
    }
}

void SLAudioDevice::getSampleFormat(SLDataFormat_PCM *pFormat) {
    memset(pFormat, 0, sizeof(*pFormat));

    pFormat->formatType = SL_DATAFORMAT_PCM;
    pFormat->numChannels = static_cast<SLuint32>(channels);
    pFormat->channelMask = getChannelMask();
    pFormat->samplesPerSec = static_cast<SLuint32>(sampleRate);

    pFormat->endianness = SL_BYTEORDER_LITTLEENDIAN;
    pFormat->bitsPerSample = static_cast<SLuint32>(format);
    pFormat->containerSize = static_cast<SLuint32>(format);
}