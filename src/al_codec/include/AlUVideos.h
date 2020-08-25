/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALUVIDEOS_H
#define HWVC_ANDROID_ALUVIDEOS_H

#include "AlUAbsMedia.h"
#include "AlMediaClip.h"
#include "AbsVideoDecoder.h"
#include "AlIdentityCreator.h"
#include "HwVideoFrame.h"
#include "AlVector.h"

al_def_unit(AlUVideos, AlUAbsMedia) {
public:
    AlUVideos(const std::string alias);

    virtual ~AlUVideos();

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
    bool _onLayerDone(AlMessage *msg);

    void _seek(std::shared_ptr<AbsDecoder> decoder, int64_t timeInUS);

    int32_t _findLayer(AlMediaClip *clip);

    void _addLayer(AlMediaClip *clip, int32_t width, int32_t height);

    void _updateLayer(AlMediaClip *clip, HwVideoFrame *frame);

    void _clearLayers(std::vector<AlID> &ignoreClips);

    void _clearLayer(int32_t layerID);

private:
    std::map<AlID, int32_t> mLayerMap;
    std::vector<AlID> ignoreClips;
};


#endif //HWVC_ANDROID_ALUVIDEOS_H
