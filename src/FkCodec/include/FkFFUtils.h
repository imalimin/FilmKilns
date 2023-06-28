/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HARDWAREVIDEOCODEC_FFUTILS_H
#define HARDWAREVIDEOCODEC_FFUTILS_H

#include "FkObject.h"
#include <pthread.h>
#include <mutex>

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

FK_SUPER_CLASS(FkFFUtils, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkFFUtils)

public:
    static int init();

    static int avSamplesCopy(AVFrame *dest, AVFrame *src);

    static void attachJvm(void *vm);

    static int exec(std::string cmd);

    static int trackInfo(std::string &file);

    static void showInfo();

private:
};

#endif //HARDWAREVIDEOCODEC_FFUTILS_H
