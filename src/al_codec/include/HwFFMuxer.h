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

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libswresample/swresample.h"

#ifdef __cplusplus
}
#endif

class HwFFMuxer : public HwAbsMuxer {
public:
    HwFFMuxer();

    virtual ~HwFFMuxer();

    virtual HwResult configure(string filePath, string type) override;

    virtual HwResult start();

    virtual int32_t addTrack(AlBundle &format) override;

    /**
     * @param packet AvPacket
     */
    virtual HwResult write(int32_t track, HwPacket *pkt) override;

private:
    void release();

    bool copyExtraData(AVStream *stream, AlBundle &format);

    void setInt32Parameter(int32_t &param, int32_t value);

    void setInt64Parameter(int64_t &param, int64_t value);

private:
    bool started = false;
    AVFormatContext *pFormatCtx = nullptr;
    vector<AVStream *> tracks;
    AVPacket *avPacket = nullptr;
};

#endif //HWVC_ANDROID_HWFFMUXER_H
