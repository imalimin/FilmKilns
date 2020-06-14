/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlVideoV2Processor.h"

#include <utility>
#include "AlUTimeline.h"
#include "AlUSequence.h"
#include "AlUAudios.h"
#include "HwSpeaker.h"

#define TAG "AlVideoV2Processor"

AlVideoV2Processor::AlVideoV2Processor() : AlAbsProcessor(TAG) {
    registerAnUnit(new AlUTimeline(ALIAS_TIMELINE));
    registerAnUnit(new AlUSequence(ALIAS_SEQUENCE));
    registerAnUnit(new AlUAudios(ALIAS_AUDIOS));
    registerAnUnit(new HwSpeaker(ALIAS_SPEAKER));
}

AlVideoV2Processor::~AlVideoV2Processor() {

}

void AlVideoV2Processor::onCreate() {
    AlAbsProcessor::onCreate();
}

void AlVideoV2Processor::onDestroy() {
    AlAbsProcessor::onDestroy();
}

int32_t AlVideoV2Processor::addTrack(AlCodec::kMediaType type, std::string path) {
    AlMessage *msg = AlMessage::obtain(MSG_SEQUENCE_TRACK_ADD);
    msg->arg1 = (int32_t) type;
    msg->desc = std::move(path);
    postMessage(msg);
    return 0;
}
