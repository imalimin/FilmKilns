/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVCSCREEN_ALENCODERBUILDER_H
#define HWVCSCREEN_ALENCODERBUILDER_H

#include "Object.h"
#include "AlSize.h"
#include "HwSampleFormat.h"
#include "HwAbsVideoEncoder.h"

al_class(AlEncoderBuilder) {
public:
    AlEncoderBuilder();

    virtual ~AlEncoderBuilder();

    AlEncoderBuilder &setSize(AlSize size);

    AlEncoderBuilder &setAudioFormat(HwSampleFormat &format);

    /// Default false.
    /// \param enable
    /// \return
    AlEncoderBuilder &setEnableHardware(bool enable);

    /// Default true.
    /// \param enable
    /// \return
    AlEncoderBuilder &setEnableAsyn(bool enable);

    AlEncoderBuilder &setBitrate(int32_t rate);

    AlEncoderBuilder &setProfile(std::string profile);

    AlEncoderBuilder &setOutput(std::string output);

    HwAbsVideoEncoder *build();

private:
    AlSize size;
    HwSampleFormat audioFormat;
    bool enableHardware = false;
    bool enableAsyn = true;
    int32_t bitrate = 0;
    std::string profile;
    std::string output;
};


#endif //HWVCSCREEN_ALENCODERBUILDER_H
