/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALCODEC_H
#define HWVC_ANDROID_ALCODEC_H

#include "Object.h"
#include "HwResult.h"
#include "HwBundle.h"
#include "HwAbsMediaFrame.h"
#include "HwPacket.h"

class AlCodec : public Object {
public:
    AL_ENUM kID : int {
        NONE = AV_CODEC_ID_NONE,
        H264 = AV_CODEC_ID_H264,
        AAC = AV_CODEC_ID_AAC
    };

    AL_ENUM kType : int {
        SOFT = 0,
        HARD,
        HARD_ENC_TEX
    };

    AL_CLASS Desc AL_EXTEND Object {
    public:
        Desc();

        Desc(kID id, kType type);

        Desc(const Desc &o);

        virtual ~Desc();

    public:
        kID id;
        kType type;
    };
public:
    AlCodec(AlCodec::kID id);

    virtual  ~AlCodec();

    virtual HwResult configure(HwBundle &format);

    virtual HwResult start() = 0;

    virtual int32_t getCodecId();

    virtual HwBundle &getFormat();

    /**
     * For encoder. HwAbsMediaFrame in & AVPacket out.
     * For decoder. AVPacket in & HwAbsMediaFrame out.
     */
    virtual HwResult process(HwAbsMediaFrame **frame, HwPacket **pkt) = 0;

    /**
     * @return 0 for video. 1 for audio. Other invalid.
     */
    virtual int32_t type() = 0;

    /**
     * @param key csd-0\csd-1\csd-2
     * @return buffer.
     */
    virtual HwBuffer *getExtraBuffer(string key) = 0;

    virtual void flush() = 0;

protected:
    AlCodec::kID id;
    HwBundle format;
public:
    static const string KEY_MIME;
    static const string KEY_FORMAT;
    static const string KEY_PROFILE;
    static const string KEY_PRESET;
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

    static const string KEY_CSD_0;
    static const string KEY_CSD_1;
    static const string KEY_CSD_2;
    static const string KEY_CSD_3;
};


#endif //HWVC_ANDROID_ALCODEC_H
