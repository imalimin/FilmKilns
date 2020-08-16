/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALPICFRAMEDECODER_H
#define HWVC_ANDROID_ALPICFRAMEDECODER_H

#include "AbsVideoDecoder.h"
#include "HwFrameAllocator.h"
#include "AlFFUtils.h"
#include "AlBundle.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"

#ifdef __cplusplus
}
#endif

AL_CLASS AlPicFrameDecoder AL_EXTEND AbsVideoDecoder {
public:
    AlPicFrameDecoder();

    virtual ~AlPicFrameDecoder();

    virtual bool prepare(string path) override;

    virtual int width() override;

    virtual int height() override;

    virtual void setOutSampleFormat(HwSampleFormat format) override;

    virtual int getChannels() override;

    virtual int getSampleHz() override;

    virtual int getSampleFormat() override;

    virtual int getSamplesPerBuffer() override;

    virtual void seek(int64_t us, AbsDecoder::kSeekMode mode = AbsDecoder::kSeekMode::BACKWARD) override;

    /**
     * @return 1: video, 2: audio, 0: failed
     */
    virtual HwResult grab(HwAbsMediaFrame **frame) override;

    virtual int64_t getVideoDuration() override;

    virtual int64_t getAudioDuration() override;

    virtual int64_t getDuration() override;

    virtual int64_t getAudioStartTime() override;

    virtual void start() override;

    virtual void pause() override;

    virtual void stop() override;

private:
    int32_t _handleAction();

    void _setupSwr();

    AVFrame *_doSwr(AVFrame *src);

    void _setupBSF();

    bool _inCurGop(int64_t timeInUS);

public:
    AlPicFrameDecoder(const AlPicFrameDecoder &o) : AbsVideoDecoder() {};

private:
    AVRational oRational = AVRational{1, 1000000};
    HwFrameAllocator *hwFrameAllocator = nullptr;
    string path;
    AVFormatContext *pFormatCtx = nullptr;
    AVCodecContext *vCtx = nullptr;
    /// For swr gif frame.
    SwsContext *sCtx = nullptr;
    AVBSFContext *bsf = nullptr;
    int vTrack = -1;
    AVPacket *vPacket = nullptr;
    AVFrame *vFrame = nullptr;
    AVFrame *vFinalFrame = nullptr;
    HwAbsMediaFrame *oHwFrame = nullptr;
    bool eof = false;
    /** action */
    AlBundle mSeekAction;
    int64_t lastPktPts = 0;
};


#endif //HWVC_ANDROID_ALPICFRAMEDECODER_H
