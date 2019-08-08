/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef HARDWAREVIDEOCODEC_ABSVIDEODECODER_H
#define HARDWAREVIDEOCODEC_ABSVIDEODECODER_H

#include "AbsAudioDecoder.h"

class AbsVideoDecoder : virtual public AbsAudioDecoder {
public:
    AbsVideoDecoder();

    virtual ~AbsVideoDecoder();

    virtual int64_t getVideoDuration()=0;

    virtual int width()=0;

    virtual int height()=0;
};


#endif //HARDWAREVIDEOCODEC_ABSVIDEODECODER_H
