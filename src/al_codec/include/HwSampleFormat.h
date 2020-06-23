//
// Created by mingyi.li on 2019/6/16.
//

#ifndef HARDWAREVIDEOCODEC_HWSAMPLEFORMAT_H
#define HARDWAREVIDEOCODEC_HWSAMPLEFORMAT_H

#include "Object.h"
#include "HwFrameFormat.h"

class HwSampleFormat : public Object {
public:
    HwSampleFormat(HwFrameFormat format, uint16_t channels, uint32_t sampleRate);

    HwSampleFormat(const HwSampleFormat &o);

    virtual ~HwSampleFormat();

    uint16_t getChannels();

    uint32_t getSampleRate();

    HwFrameFormat getFormat();

    int32_t getAVFormat();

    HwSampleFormat &operator=(const HwSampleFormat &o);

    bool operator==(const HwSampleFormat &o);

    bool operator!=(const HwSampleFormat &o);

    size_t getBytesPerSample();

    bool valid();

private:
    uint16_t channels = 0;
    uint32_t sampleRate = 0;
    HwFrameFormat format = HwFrameFormat::HW_FMT_NONE;

public:
    static const HwSampleFormat NONE;
};


#endif //HARDWAREVIDEOCODEC_HWSAMPLEFORMAT_H
