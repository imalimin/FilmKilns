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

public:
    static HwResult remux(std::string input, std::string output,
                          std::vector<int64_t> trimIns,
                          std::vector<int64_t> trimOuts);

private:
    static bool contains(std::vector<int64_t> *trimIns, std::vector<int64_t> *trimOuts,
                         int64_t pts);

    static bool createCodec(AVStream *stream, AVCodecContext **eCtx, AVCodecContext **dCtx);
};


#endif //HWVC_ANDROID_HWVIDEOUTILS_H
