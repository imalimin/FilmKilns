/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HARDWAREVIDEOCODEC_FFUTILS_H
#define HARDWAREVIDEOCODEC_FFUTILS_H

#include "Object.h"
#include "AlMediaCoverSequence.h"
#include <pthread.h>
#include <mutex>
#include <include/AlBuffer.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libswresample/swresample.h"
#include "libavcodec/jni.h"

#ifdef __cplusplus
}
#endif

static std::once_flag onceFlag;

AL_CLASS AlFFUtils AL_EXTEND Object {
public:
    static int init();

    static int avSamplesCopy(AVFrame *dest, AVFrame *src);

    static void attachJvm(void *vm);

    static int exec(std::string cmd);

    static int trackInfo(std::string &file);

    static int parseWaveform(int64_t seqIn, int64_t duInUS,
                             std::vector<std::string> &files,
                             std::vector<int64_t> &seqIns,
                             std::vector<int64_t> &trimIns,
                             std::vector<int64_t> &dus,
                             float *data,
                             int size);

    static std::shared_ptr<AlMediaCoverSequence> parseVideoCover(int64_t seqIn, int64_t duInUS,
                                                                 std::vector<std::string> &files,
                                                                 std::vector<int64_t> &seqIns,
                                                                 std::vector<int64_t> &trimIns,
                                                                 std::vector<int64_t> &dus,
                                                                 int width);

    static void showInfo();

private:
};

#endif //HARDWAREVIDEOCODEC_FFUTILS_H
