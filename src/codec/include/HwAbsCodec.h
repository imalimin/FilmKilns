/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWABSCODEC_H
#define HWVC_ANDROID_HWABSCODEC_H

#include "Object.h"
#include "HwResult.h"
#include "HwBundle.h"
#include "HwAbsMediaFrame.h"
#include "HwPacket.h"

class HwAbsCodec : public Object {
public:
    HwAbsCodec(int32_t codecId);

    virtual  ~HwAbsCodec();

    virtual HwResult configure(HwBundle *format);

    virtual HwResult start()=0;

    virtual int32_t getCodecId();

    virtual HwBundle *getFormat();

    /**
     * For encoder. HwAbsMediaFrame in & AVPacket out.
     * For decoder. AVPacket in & HwAbsMediaFrame out.
     */
    virtual HwResult process(HwAbsMediaFrame **frame, HwPacket **pkt)=0;

    /**
     * @return 0 for video. 1 for audio. Other invalid.
     */
    virtual int32_t type()=0;

    /**
     * @param key csd-0\csd-1\csd-2
     * @return buffer.
     */
    virtual HwBuffer *getExtraBuffer(string key)=0;

    virtual void flush()=0;

protected:
    int32_t codecId = INT32_MIN;
    HwBundle *format = nullptr;
public:
    static const string KEY_MIME;
    static const string KEY_FORMAT;
    static const string KEY_PROFILE;
    static const string KEY_LEVEL;
    static const string KEY_BIT_RATE;
    // Video
    static const string KEY_WIDTH;
    static const string KEY_HEIGHT;
    static const string KEY_FPS;
    static const string KEY_QUALITY;
    //Audio
    static const string KEY_SAMPLE_RATE;
    static const string KEY_CHANNELS;
    static const string KEY_FRAME_SIZE;
};


#endif //HWVC_ANDROID_HWABSCODEC_H
