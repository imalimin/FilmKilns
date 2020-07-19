/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALUVIDEOS_H
#define HWVC_ANDROID_ALUVIDEOS_H

#include "Unit.h"
#include "AlMediaClip.h"
#include "AbsVideoDecoder.h"
#include "AlIdentityCreator.h"
#include "HwVideoFrame.h"
#include "AlVector.h"

al_def_unit(AlUVideos, Unit) {
public:
    AlUVideos(const std::string alias);

    virtual ~AlUVideos();

    virtual bool onCreate(AlMessage *msg) override;

    virtual bool onDestroy(AlMessage *msg) override;

private:
    bool _onAddTrack(AlMessage *msg);

    bool _onRemoveTrack(AlMessage *msg);

    bool _onBeat(AlMessage *msg);

    bool _onEnd(AlMessage *msg);

    bool _onLayerDone(AlMessage *msg);

    void _create(AlMediaClip *clip, int64_t &duration, int64_t &frameDuration);

    void _seek(AbsVideoDecoder *decoder, int64_t timeInUS);

    AbsVideoDecoder *_findDecoder(AlMediaClip *clip);

    int32_t _findLayer(AlMediaClip *clip);

    void _addLayer(AlMediaClip *clip, int32_t width, int32_t height);

    void _updateLayer(AlMediaClip *clip, HwVideoFrame *frame);

    HwResult _grab(AlMediaClip *clip, AbsVideoDecoder *decoder,
                   HwAbsMediaFrame **frame, int64_t timeInUS);

    HwResult _correct(AlMediaClip *clip, AbsVideoDecoder *decoder);

    void _setCurTimestamp(AlMediaClip *clip, int64_t timeInUS);

    int64_t _getCurTimestamp(AlMediaClip *clip);

    void _clearLayers(std::vector<AlID> &ignoreClips);

    void _clearLayer(int32_t layerID);

private:
    std::map<AlID, std::unique_ptr<AbsVideoDecoder>> map;
    std::map<AlID, int64_t> mCurTimeMap;
    std::map<AlID, int32_t> mLayerMap;
    std::map<AlID, HwAbsMediaFrame *> mLastFrameMap;
    int64_t mCurTimeInUS = 0;
};


#endif //HWVC_ANDROID_ALUVIDEOS_H
