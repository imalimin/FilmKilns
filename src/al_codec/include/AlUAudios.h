/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALUAUDIOS_H
#define HWVC_ANDROID_ALUAUDIOS_H

#include "Unit.h"
#include "AlMediaClip.h"
#include "AbsAudioDecoder.h"
#include "AlIdentityCreator.h"
#include <map>

al_def_unit(AlUAudios, Unit) {
public:
    AlUAudios(const std::string alias);

    virtual ~AlUAudios();

    virtual bool onCreate(AlMessage *msg) override;

    virtual bool onDestroy(AlMessage *msg) override;

private:
    bool _onAddTrack(AlMessage *msg);

    bool _onBeat(AlMessage *msg);

    void _create(AlMediaClip *clip, int64_t &duration, int64_t &frameDuration);

    AbsAudioDecoder *_findDecoder(AlMediaClip *clip);

    void _seek(AbsAudioDecoder *decoder, int64_t timeInUS);

private:
    std::map<AlID, std::unique_ptr<AbsAudioDecoder>> map;
};


#endif //HWVC_ANDROID_ALUAUDIOS_H
