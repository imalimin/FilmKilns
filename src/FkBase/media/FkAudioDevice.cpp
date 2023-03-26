/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/
#include "FkAudioDevice.h"

FK_IMPL_CLASS_TYPE(FkAudioDevice, FkObject)
FK_IMPL_CLASS_TYPE(FkSLAudioDevice, FkAudioDevice)

FkAudioDevice::FkAudioDevice(uint16_t channels,
                             uint32_t sampleRate,
                             uint16_t format,
                             uint32_t samplesPerBuffer)
        : FkObject(), channels(channels),
          sampleRate(sampleRate),
          format(format),
          samplesPerBuffer(samplesPerBuffer) {
}

uint16_t FkAudioDevice::getChannels() {
    return channels;
}

uint32_t FkAudioDevice::getSampleRate() {
    return sampleRate;
}

uint16_t FkAudioDevice::getFormat() {
    return format;
}

uint32_t FkAudioDevice::getSamplesPerBuffer() {
    return samplesPerBuffer;
}

uint32_t FkAudioDevice::getBufferByteSize() {
    uint32_t bufSize = samplesPerBuffer * channels * format;
    bufSize = (bufSize + 7) >> 3;  // bits --> byte
    return bufSize;
}

FkSLAudioDevice::FkSLAudioDevice(FkAudioDevice::kMode mode,
                                 uint16_t channels,
                                 uint32_t sampleRate,
                                 uint16_t format,
                                 uint32_t samplesPerBuffer)
        : FkAudioDevice(channels, sampleRate, format, samplesPerBuffer),
          mode(mode) {
}

SLuint32 FkSLAudioDevice::getChannelMask() {
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

void FkSLAudioDevice::getSampleFormat(SLDataFormat_PCM *pFormat) {
    memset(pFormat, 0, sizeof(*pFormat));

    pFormat->formatType = SL_DATAFORMAT_PCM;
    pFormat->numChannels = static_cast<SLuint32>(channels);
    pFormat->channelMask = getChannelMask();
    pFormat->samplesPerSec = static_cast<SLuint32>(sampleRate);

    pFormat->endianness = SL_BYTEORDER_LITTLEENDIAN;
    pFormat->bitsPerSample = static_cast<SLuint32>(format);
    pFormat->containerSize = static_cast<SLuint32>(format);
}