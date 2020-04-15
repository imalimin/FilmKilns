/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWVIDEOUTILS_H
#define HWVC_ANDROID_HWVIDEOUTILS_H

#include "Object.h"
#include "HwResult.h"
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif

#include "../include/ff/libavcodec/avcodec.h"
#include "../include/ff/libavformat/avformat.h"
#include "../include/ff/libavutil/avutil.h"
#include "../include/ff/libswresample/swresample.h"

#ifdef __cplusplus
}
#endif

class HwVideoUtils : public Object {
public:
    class Context : public Object {
    public:
        AVFormatContext *c;
        int32_t vTrackIndex = -1;
        int32_t aTrackIndex = -1;
    public:
        static Context *open(std::string path);

        static Context *open(std::string path, Context *iCtx);

        ~Context();

    private:
        Context();

        Context(Context &context) {}
    };

    class RecodeContext : public Object {
    public:
        static const int32_t FLAG_NONE = 0;
        static const int32_t FLAG_LAST_PKT_SKIPPED = 1;
        static const int32_t FLAG_RECODING = 2;
        AVCodecContext *dCtx = nullptr, *eCtx = nullptr;
        int32_t gopIndex = -1;
        int32_t flag = FLAG_NONE;
    };

public:
    static HwResult remuxCopy(std::string input, std::string output,
                          std::vector<int64_t> trimIns,
                          std::vector<int64_t> trimOuts);

    static HwResult remux(std::string input, std::string output,
                          std::vector<int64_t> trimIns,
                          std::vector<int64_t> trimOuts);

private:
    static bool contains(std::vector<int64_t> *trimIns, std::vector<int64_t> *trimOuts,
                         int64_t pts);

    static bool createCodec(AVStream *stream, AVCodecContext **dCtx, AVCodecContext **eCtx);

    static bool recode(AVPacket **pkt, Context *iCtx, RecodeContext *rCtx);
};


#endif //HWVC_ANDROID_HWVIDEOUTILS_H
