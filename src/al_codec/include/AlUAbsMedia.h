/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALUABSMEDIA_H
#define HWVC_ANDROID_ALUABSMEDIA_H

#include "Unit.h"
#include "AlMediaTrack.h"
#include "AlMediaClip.h"
#include "AbsDecoder.h"
#include "AlIdentityCreator.h"
#include "AlVector.h"
#include <map>

al_def_unit(AlUAbsMedia, Unit) {
public:
    AlUAbsMedia(const std::string alias, AlMediaTrack::kType type);

    virtual ~AlUAbsMedia();

    virtual bool onCreate(AlMessage *msg) override;

    virtual bool onDestroy(AlMessage *msg) override;

protected:
    virtual std::shared_ptr<AbsDecoder> createDecoder(AlMediaClip *clip,
                                                      int64_t &frameDuration) = 0;

    virtual bool shouldDecodeFrame() = 0;

    virtual bool onInterruptClip(AlMediaClip *clip) = 0;

    virtual bool onClipEOF(AlMediaClip *clip) = 0;

    virtual bool onDispatchFrame(AlMediaClip *clip, HwAbsMediaFrame *frame, int64_t timeInUS) = 0;

    virtual bool onBeatFinish(std::shared_ptr<AlVector<std::shared_ptr<AlMediaClip>>> clips) = 0;

    virtual void onActionSeek(std::shared_ptr<AlVector<std::shared_ptr<AlMediaClip>>> clips, int64_t timeInUS) = 0;

    std::shared_ptr<AbsDecoder> findDecoderByClip(AlMediaClip *clip);

protected:
    bool _onAddTrack(AlMessage *msg);

    bool _onRemoveTrack(AlMessage *msg);

    bool _onEnd(AlMessage *msg);

    bool _onBeat(AlMessage *msg);

private:
    void _create(AlMediaClip *clip, int64_t &duration, int64_t &frameDuration);

    HwResult _grab(AlMediaClip *clip, std::shared_ptr<AbsDecoder> decoder,
                   HwAbsMediaFrame **frame, int64_t timeInUS);

    int64_t _transPts(AlMediaClip *clip, int64_t pts);

private:
    AlMediaTrack::kType type;
    std::map<AlID, std::shared_ptr<AbsDecoder>> map;
    std::map<AlID, HwAbsMediaFrame *> mLastFrameMap;
    int64_t mCurTimeInUS = 0;
};


#endif //HWVC_ANDROID_ALUABSMEDIA_H
