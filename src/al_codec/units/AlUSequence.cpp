/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlUSequence.h"
#include "AlCodec.h"

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
    auto array = std::make_shared<AlVector<std::shared_ptr<AlMediaClip>>>();
    _findClipsByTime(*array, msg->arg2);
    if (!array->empty()) {
        auto *msg1 = AlMessage::obtain(MSG_SEQUENCE_BEAT_AUDIO);
        msg1->sp = array;
        postMessage(msg1);
    }
    return true;
}

bool AlUSequence::_onAddTrack(AlMessage *msg) {
    auto type = (AlCodec::kMediaType) msg->arg1;
    AlFileDescriptor descriptor(msg->desc);
    AlID id = creator.generate();
    auto track = std::make_unique<AlMediaTrack>(id, type);
    track->addClip(creator.generate(), descriptor);
    tracks.insert(std::make_pair(id, std::move(track)));
    AlLogI(TAG, "type(%d), path(%s)", type, msg->desc.c_str());
    return true;
}


void AlUSequence::_findClipsByTime(AlVector<std::shared_ptr<AlMediaClip>> &array,
                                   int64_t timeInUS) {
    auto itr = this->tracks.begin();
    while (this->tracks.end() != itr) {
        itr->second->findClips(array, timeInUS);
    }
}
