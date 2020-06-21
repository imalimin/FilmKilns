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
    auto clips = std::make_shared<AlVector<std::shared_ptr<AlMediaClip>>>();
    _findClipsByTime(*clips, msg->arg2);
    if (!clips->empty()) {
        auto *msg1 = AlMessage::obtain(MSG_SEQUENCE_BEAT_AUDIO);
        msg1->sp = clips;
        postMessage(msg1);
    }
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
    auto tmp = std::static_pointer_cast<AlMediaClip>(msg->sp);
    AlID id = tmp->id();
    int64_t duration = tmp->getDuration();
    AlLogI(TAG, "id(%d), duration(%" PRId64 ")", id, duration);
    auto *clip = _findClip(id);
    if (clip) {
        clip->setSeqIn(0);
        clip->setTrimIn(0);
        clip->setDuration(duration);
        clip->setFrameDuration(tmp->getFrameDuration());
        _notifyTimeline();
    }
    return true;
}

AlMediaClip *AlUSequence::_findClip(AlID id) {
    for (auto &track : this->tracks) {
        auto *clip = track.second->findClip(id);
        if (clip) {
            return clip;
        }
    }
    return nullptr;
}

void AlUSequence::_findClipsByTime(AlVector<std::shared_ptr<AlMediaClip>> &array,
                                   int64_t timeInUS) {
    for (auto &track : this->tracks) {
        track.second->findClips(array, timeInUS);
    }
}

void AlUSequence::_notifyTimeline() {
    int32_t hzInUS = INT32_MAX;
    int64_t durationInUS = 0;
    for (auto &track : this->tracks) {
        hzInUS = std::min((int32_t) track.second->getMinFrameDuration(), hzInUS);
        durationInUS = std::max(track.second->getSeqOut(), durationInUS);
    }
    auto *msg0 = AlMessage::obtain(MSG_TIMELINE_SET_DURATION);
    msg0->arg2 = durationInUS;
    postMessage(msg0);
    auto *msg1 = AlMessage::obtain(MSG_TIMELINE_SET_HZ_IN_US);
    msg1->arg1 = hzInUS;
    postMessage(msg1);
}