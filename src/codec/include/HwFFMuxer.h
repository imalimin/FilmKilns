/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWFFMUXER_H
#define HWVC_ANDROID_HWFFMUXER_H

#include "HwAbsMuxer.h"
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif

#include "ff/libavcodec/avcodec.h"
#include "ff/libavformat/avformat.h"
#include "ff/libavutil/avutil.h"
#include "ff/libswresample/swresample.h"

class HwFFMuxer : public HwAbsMuxer {
public:
    HwFFMuxer(string filePath, string type);

    virtual ~HwFFMuxer();

    virtual HwResult start();

    virtual int32_t addAudioTrack(int32_t sampleRate) override;

    virtual int32_t addVideoTrack(int32_t fps) override;

    virtual bool write(int32_t track) override;

private:
    void release();

private:
    AVFormatContext *pFormatCtx = nullptr;
    vector<AVStream *> tracks;
};

#ifdef __cplusplus
}
#endif

#endif //HWVC_ANDROID_HWFFMUXER_H
