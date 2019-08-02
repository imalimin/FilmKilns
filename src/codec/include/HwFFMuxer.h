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
    HwFFMuxer();

    virtual ~HwFFMuxer();

    virtual HwResult configure(string filePath, string type) override;

    virtual HwResult start();

    virtual int32_t addTrack(HwAbsCodec *codec) override;

    /**
     * @param packet AvPacket
     */
    virtual HwResult write(int32_t track, void *packet) override;

private:
    void release();

    bool copyExtraData(AVStream *stream, HwAbsCodec *codec);

private:
    AVFormatContext *pFormatCtx = nullptr;
    vector<AVStream *> tracks;
};

#ifdef __cplusplus
}
#endif

#endif //HWVC_ANDROID_HWFFMUXER_H
