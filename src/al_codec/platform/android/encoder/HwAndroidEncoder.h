/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWANDROIDENCODER_H
#define HWVC_ANDROID_HWANDROIDENCODER_H

#include "../../../include/HwAbsVideoEncoder.h"
#include "../../../include/HwAbsCodec.h"
#include "../../../include/HwAbsMuxer.h"
#include <mutex>


#ifdef __cplusplus
extern "C" {
#endif

#include "../../../include/ff/libavcodec/avcodec.h"
#include "../../../include/ff/libavformat/avformat.h"
#include "../../../include/ff/libavutil/avutil.h"
#include "../../../include/ff/libswresample/swresample.h"

#ifdef __cplusplus
}
#endif

class HwAndroidEncoder : public HwAbsVideoEncoder {
public:
    HwAndroidEncoder();

    virtual ~HwAndroidEncoder();

    virtual void setBitrate(int32_t rate) override;

    virtual void setProfile(std::string profile) override;

    virtual void setPreset(std::string preset) override;

    virtual bool prepare(string path, int width, int height, HwSampleFormat audioFormat) override;

    virtual HwResult write(HwAbsMediaFrame *frame) override;

    virtual bool stop() override;

    virtual void release() override;

private:
    bool configure();

    void flush();

private:
    string path;
    int width = 0, height = 0;
    HwSampleFormat audioFormat = HwSampleFormat::NONE;
    HwAbsCodec *vCodec = nullptr;
    HwAbsCodec *aCodec = nullptr;
    HwAbsMuxer *muxer = nullptr;
    int32_t vTrack = HwAbsMuxer::TRACK_NONE, aTrack = HwAbsMuxer::TRACK_NONE;
    bool firstVideoFrameWrite = false;
    std::mutex lock;

    int64_t countOfFrame = 0;
    int64_t lastTime = 0;
    int64_t countOfTime = 0;
};


#endif //HWVC_ANDROID_HWANDROIDENCODER_H
