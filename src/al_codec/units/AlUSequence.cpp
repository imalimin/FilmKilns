/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlUSequence.h"

#define TAG "AlUSequence"

AlUSequence::AlUSequence(const std::string alias) : Unit(alias) {
    al_reg_msg(MSG_TIMELINE_HEARTBEAT, AlUSequence::_onHeartbeat);
    al_reg_msg(MSG_SEQUENCE_TRACK_ADD, AlUSequence::_onAddTrack);
}

AlUSequence::~AlUSequence() {

}

bool AlUSequence::onCreate(AlMessage *msg) {
    return true;
}

bool AlUSequence::onDestroy(AlMessage *msg) {
    return true;
}

bool AlUSequence::_onHeartbeat(AlMessage *msg) {
    AlLogD(TAG, "");
    return true;
}

bool AlUSequence::_onAddTrack(AlMessage *msg) {
    AlID id = creator.generate();
    AlMediaTrack *track = new AlMediaTrack();
    return true;
}
