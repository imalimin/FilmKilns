/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALUAUDIOS_H
#define HWVC_ANDROID_ALUAUDIOS_H

#include "AlUAbsMedia.h"
#include "AlAudioPoolMixer.h"
#include "AlMediaClip.h"
#include "AbsAudioDecoder.h"
#include "AlIdentityCreator.h"
#include "AlBuffer.h"
#include "AlVector.h"

al_def_unit(AlUAudios, AlUAbsMedia) {
public:
    AlUAudios(const std::string alias);

    virtual ~AlUAudios();

    virtual bool onCreate(AlMessage *msg) override;

    virtual bool onDestroy(AlMessage *msg) override;

protected:
    virtual std::shared_ptr<AbsDecoder> createDecoder(AlMediaClip *clip,
                                                      int64_t &frameDuration) override;

    virtual bool shouldDecodeFrame() override;

    virtual bool onInterruptClip(AlMediaClip *clip) override;

    virtual bool onClipEOF(AlMediaClip *clip) override;

    virtual bool onDispatchFrame(AlMediaClip *clip, HwAbsMediaFrame *frame, int64_t timeInUS) override;

    virtual bool onBeatFinish(std::shared_ptr<AlVector<std::shared_ptr<AlMediaClip>>> clips) override;

    virtual void onActionSeek(std::shared_ptr<AlVector<std::shared_ptr<AlMediaClip>>> clips, int64_t timeInUS) override;

private:
    void _seek(std::shared_ptr<AbsDecoder> decoder, int64_t timeInUS);

    HwResult _putSilence(AlMediaClip *clip, int nbSamples);

private:
    const int FRAME_SIZE = 1024;
    HwSampleFormat format;
    AlAudioPoolMixer *mixer = nullptr;
    AlBuffer *pSilenceBuf = nullptr;
};


#endif //HWVC_ANDROID_ALUAUDIOS_H
