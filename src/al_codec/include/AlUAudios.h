/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALUAUDIOS_H
#define HWVC_ANDROID_ALUAUDIOS_H

#include "Unit.h"
#include "AlAudioPoolMixer.h"
#include "AlMediaClip.h"
#include "AbsAudioDecoder.h"
#include "AlIdentityCreator.h"
#include "AlBuffer.h"
#include "AlVector.h"
#include <map>

al_def_unit(AlUAudios, Unit) {
public:
    AlUAudios(const std::string alias);

    virtual ~AlUAudios();

    virtual bool onCreate(AlMessage *msg) override;

    virtual bool onDestroy(AlMessage *msg) override;

private:
    bool _onAddTrack(AlMessage *msg);

    bool _onRemoveTrack(AlMessage *msg);

    bool _onBeat(AlMessage *msg);

    bool _onEnd(AlMessage *msg);

    void _seek(std::shared_ptr<AlVector<std::shared_ptr<AlMediaClip>>> clips, int64_t timeInUS);

    void _seek(AbsAudioDecoder *decoder, int64_t timeInUS);

    void _create(AlMediaClip *clip, int64_t &duration, int64_t &frameDuration);

    AbsAudioDecoder *_findDecoder(AlMediaClip *clip);

    HwResult _putSilence(AlMediaClip *clip, int nbSamples);

    HwResult _grab(AlMediaClip *clip, AbsAudioDecoder *decoder,
                   HwAbsMediaFrame **frame, int64_t timeInUS);

    void _setCurTimestamp(AlMediaClip *clip, int64_t timeInUS);

    int64_t _getCurTimestamp(AlMediaClip *clip);

private:
    const int FRAME_SIZE = 1024;
    std::map<AlID, std::unique_ptr<AbsAudioDecoder>> map;
    std::map<AlID, int64_t> mCurTimeMap;
    HwSampleFormat format;
    AlAudioPoolMixer *mixer = nullptr;
    int64_t mCurTimeInUS = 0;
    AlBuffer *pSilenceBuf = nullptr;
    std::map<AlID, HwAbsMediaFrame *> mLastFrameMap;
};


#endif //HWVC_ANDROID_ALUAUDIOS_H
