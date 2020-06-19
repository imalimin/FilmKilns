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

    int64_t _create(AlMediaClip *clip);

    AbsAudioDecoder *_findDecoder(AlMediaClip *clip);

private:
    std::map<AlID, std::unique_ptr<AbsAudioDecoder>> map;
};


#endif //HWVC_ANDROID_ALUAUDIOS_H
