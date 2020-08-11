/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALUSPEAKER_H
#define HWVC_ANDROID_ALUSPEAKER_H

#include "Unit.h"
#include "AlAudioPlayer.h"
#include "HwAudioFrame.h"

al_def_unit(AlUSpeaker, Unit) {
public:
    AlUSpeaker(string alias);

    AlUSpeaker(string alias, AudioDevice::kMode mode);

    virtual ~AlUSpeaker();

    bool onDestroy(AlMessage *msg) override;

    bool onCreate(AlMessage *msg) override;

    bool eventFeed(AlMessage *msg);

private:
    AlUSpeaker(const AlUSpeaker &o) : Unit("AlUSpeaker") {};

    void createFromAudioFrame(HwAudioFrame *frame);

private:
    AudioDevice::kMode mode = AudioDevice::kMode::Normal;
    AlAudioPlayer *player = nullptr;

};


#endif //HWVC_ANDROID_ALUSPEAKER_H
