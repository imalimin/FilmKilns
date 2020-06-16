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
    al_reg_msg(MSG_AUDIOS_TRACK_ADD_NOTIFY, AlUSequence::_onAddTrackDone);
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
//    auto array = std::make_shared<AlVector<std::shared_ptr<AlMediaClip>>>();
//    _findClipsByTime(*array, msg->arg2);
//    if (!array->empty()) {
//        auto *msg1 = AlMessage::obtain(MSG_SEQUENCE_BEAT_AUDIO);
//        msg1->sp = array;
//        postMessage(msg1);
//    }
    return true;
}

bool AlUSequence::_onAddTrack(AlMessage *msg) {
    auto type = (AlCodec::kMediaType) msg->arg1;
    AlFileDescriptor descriptor(msg->desc);
    auto track = std::make_unique<AlMediaTrack>(creator.generate(), type);
    AlID clipID = track->addClip(creator.generate(), descriptor);
    auto clip = track->findClip(clipID);
    if (clip) {
        auto *msg1 = AlMessage::obtain(MSG_AUDIOS_ADD);
        msg1->sp = std::make_shared<AlMediaClip>(*clip);
        postMessage(msg1);
    }
    tracks.insert(std::make_pair(track->id(), std::move(track)));
    AlLogI(TAG, "type(%d), path(%s)", type, msg->desc.c_str());
    return true;
}

bool AlUSequence::_onAddTrackDone(AlMessage *msg) {
    AlID id = msg->arg1;
    int64_t duration = msg->arg2;
    AlLogI(TAG, "id(%d), duration(%" PRId64 ")", id, duration);
    auto *clip = _findClip(id);
    if (clip) {
        clip->setSeqIn(0);
        clip->setTrimIn(0);
        clip->setDuration(duration);
        _notifyTimeline();
    }
    return true;
}

AlMediaClip *AlUSequence::_findClip(AlID id) {
    auto itr = this->tracks.begin();
    while (this->tracks.end() != itr) {
        auto *clip = itr->second->findClip(id);
        if (clip) {
            return clip;
        }
    }
    return nullptr;
}

void AlUSequence::_findClipsByTime(AlVector<std::shared_ptr<AlMediaClip>> &array,
                                   int64_t timeInUS) {
    auto itr = this->tracks.begin();
    while (this->tracks.end() != itr) {
        itr->second->findClips(array, timeInUS);
    }
}

void AlUSequence::_notifyTimeline() {
    int32_t hzInUS = 0;
    int64_t durationInUS = 0;
    auto itr = this->tracks.begin();
    while (this->tracks.end() != itr) {
        durationInUS = std::max(itr->second->getSeqOut(), durationInUS);
    }
    auto *msg0 = AlMessage::obtain(MSG_TIMELINE_SET_DURATION);
    msg0->arg2 = durationInUS;
    postMessage(msg0);
    auto *msg1 = AlMessage::obtain(MSG_TIMELINE_SET_HZ_IN_US);
    msg1->arg1 = hzInUS;
    postMessage(msg1);
}