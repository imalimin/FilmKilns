/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/
#ifndef FK_BASE_FKAUDIODEVICE_H
#define FK_BASE_FKAUDIODEVICE_H

#include "FkObject.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>


FK_SUPER_CLASS(FkAudioDevice, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkAudioDevice)

public:
    enum class kMode : int {
        LowLatency,
        Normal,
        HighLatency
    };
public:
    FkAudioDevice(uint16_t channels,
                  uint32_t sampleRate,
                  uint16_t format,
                  uint32_t samplesPerBuffer);

    virtual uint16_t getChannels();

    virtual uint32_t getSampleRate();

    virtual uint16_t getFormat();

    virtual uint32_t getSamplesPerBuffer();

    virtual uint32_t getBufferByteSize();

protected:
    uint16_t channels = 0;
    uint32_t sampleRate = 0;
    uint16_t format = SL_PCMSAMPLEFORMAT_FIXED_16;
    uint32_t samplesPerBuffer = 0;//sampleCount
};

FK_SUPER_CLASS(FkSLAudioDevice, FkAudioDevice) {
FK_DEF_CLASS_TYPE_FUNC(FkSLAudioDevice)

public:
    FkSLAudioDevice(FkAudioDevice::kMode mode,
                    uint16_t channels,
                    uint32_t sampleRate,
                    uint16_t format,
                    uint32_t samplesPerBuffer);

    SLuint32 getChannelMask();

    void getSampleFormat(SLDataFormat_PCM *pFormat);

protected:
    FkAudioDevice::kMode mode = FkAudioDevice::kMode::Normal;
};


#endif //FK_BASE_FKAUDIODEVICE_H
