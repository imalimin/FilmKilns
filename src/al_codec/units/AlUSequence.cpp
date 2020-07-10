/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlUSequence.h"
#include "AlFuture.h"

#define TAG "AlUSequence"

AlUSequence::AlUSequence(const std::string alias) : Unit(alias) {
    al_reg_msg(MSG_TIMELINE_HEARTBEAT, AlUSequence::_onHeartbeat);
    al_reg_msg(MSG_TIMELINE_END, AlUSequence::_onTimelineEnd);
    al_reg_msg(MSG_SEQUENCE_TRACK_ADD, AlUSequence::_onAddTrack);
    al_reg_msg(MSG_SEQUENCE_TRACK_REMOVE, AlUSequence::_onRemoveTrack);
    al_reg_msg(MSG_SEQUENCE_TRACK_SET_DURATION, AlUSequence::_onSetTrackDuration);
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
    _beatAudioClips(msg->arg2);
    _beatVideoClips(msg->arg2);
    return true;
}

bool AlUSequence::_onAddTrack(AlMessage *msg) {
    auto type = (AlMediaTrack::kType) msg->arg1;
    auto bundle = std::static_pointer_cast<AlFuture>(msg->sp);
    auto tmp = std::static_pointer_cast<AlMediaClip>(bundle->sp);

    auto track = std::make_unique<AlMediaTrack>(creator.generate(), type);
    AlID clipID = track->addClip(creator.generate(), *(tmp->getInputDescriptor()));
    auto *clip = track->findClip(clipID);
    if (clip) {
        clip->setSeqIn(tmp->getSeqIn());
        clip->setTrimIn(tmp->getTrimIn());
        auto *msg1 = AlMessage::obtain(MSG_AUDIOS_TRACK_ADD);
        msg1->sp = std::make_shared<AlMediaClip>(*clip);
        postMessage(msg1);
    }
    bundle->put(track->id());
    auto *msg2 = AlMessage::obtain(MSG_SEQUENCE_TRACK_ADD_DONE);
    msg2->arg1 = track->id();
    postMessage(msg2);
    tracks.insert(std::make_pair(track->id(), std::move(track)));
    AlLogI(TAG, "type(%d), path(%s)", type, msg->desc.c_str());
    return true;
}

bool AlUSequence::_onRemoveTrack(AlMessage *msg) {
    auto itr = tracks.find(msg->arg1);
    if (tracks.end() != itr) {
        auto clips = std::make_shared<AlVector<std::shared_ptr<AlMediaClip>>>();
        itr->second->findAllClips(*clips);
        tracks.erase(itr);
        auto *msg1 = AlMessage::obtain(MSG_AUDIOS_TRACK_REMOVE);
        msg1->sp = clips;
        postMessage(msg1);
    }
    return true;
}

bool AlUSequence::_onSetTrackDuration(AlMessage *msg) {
    auto tmp = std::static_pointer_cast<AlMediaClip>(msg->sp);
    AlID id = tmp->id();
    int64_t duration = tmp->getDuration();
    AlLogI(TAG, "id(%d), duration(%" PRId64 ")", id, duration);
    auto *clip = _findClip(id);
    if (clip) {
        clip->setDuration(duration);
        clip->setFrameDuration(tmp->getFrameDuration());
        _notifyTimeline();
    }
    return true;
}

bool AlUSequence::_onTimelineEnd(AlMessage *msg) {
    auto aClips = std::make_shared<AlVector<std::shared_ptr<AlMediaClip>>>();
    for (auto &track : this->tracks) {
        if (AlMediaTrack::kType::AUDIO == track.second->type()) {
            track.second->findAllClips(*aClips);
        }
    }
    if (!aClips->empty()) {
        auto *msg1 = AlMessage::obtain(MSG_AUDIOS_END);
        msg1->sp = aClips;
        postMessage(msg1);
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

void AlUSequence::_findClipsByTime(AlMediaTrack::kType type,
                                   AlVector<std::shared_ptr<AlMediaClip>> &array,
                                   int64_t timeInUS) {
    for (auto &track : this->tracks) {
        if (type == track.second->type()) {
            track.second->findClips(array, timeInUS);
        }
    }
}

AlMediaClip *AlUSequence::_findAudioRefClip() {
    auto aClips = std::make_shared<AlVector<std::shared_ptr<AlMediaClip>>>();
    for (auto &track : this->tracks) {
        if (AlMediaTrack::kType::AUDIO_REF == track.second->type()) {
            track.second->findAllClips(*aClips);
        }
    }
    return aClips->empty() ? nullptr : aClips->begin()->get();
}

void AlUSequence::_notifyTimeline() {
    int32_t hzInUS = INT32_MAX;
    int64_t durationInUS = 0;
    for (auto &track : this->tracks) {
        if (AlMediaTrack::kType::VIDEO == track.second->type() ||
            AlMediaTrack::kType::AUDIO == track.second->type()) {
            hzInUS = std::min((int32_t) track.second->getMinFrameDuration(), hzInUS);
            durationInUS = std::max(track.second->getSeqOut(), durationInUS);
        }
    }

    auto *refClip = _findAudioRefClip();
    if (nullptr == refClip) {
        auto track = std::make_unique<AlMediaTrack>(creator.generate(),
                                                    AlMediaTrack::kType::AUDIO_REF);
        AlID clipID = track->addClip(creator.generate(), AlFileDescriptor::EMPTY);
        refClip = track->findClip(clipID);
        if (refClip) {
            refClip->setSeqIn(0);
            refClip->setTrimIn(0);
            refClip->setFrameDuration(1024 * 1e6 / 44100);
            tracks.insert(std::make_pair(track->id(), std::move(track)));
        }
    }
    if (refClip) {
        refClip->setDuration(durationInUS);
    }

    auto *msg0 = AlMessage::obtain(MSG_TIMELINE_SET_DURATION);
    msg0->arg2 = durationInUS;
    postMessage(msg0);
    auto *msg1 = AlMessage::obtain(MSG_TIMELINE_SET_HZ_IN_US);
    msg1->arg1 = hzInUS;
    postMessage(msg1);
}

void AlUSequence::_beatAudioClips(int64_t timeInUS) {
    auto clips = std::make_shared<AlVector<std::shared_ptr<AlMediaClip>>>();
    _findClipsByTime(AlMediaTrack::kType::AUDIO, *clips, timeInUS);
    auto *refClip = _findAudioRefClip();
    if (refClip) {
        clips->push_back(std::make_shared<AlMediaClip>(*refClip));
    }
    if (!clips->empty()) {
        auto *msg1 = AlMessage::obtain(MSG_SEQUENCE_BEAT_AUDIO);
        msg1->arg2 = timeInUS;
        msg1->sp = clips;
        postMessage(msg1);
    }
}

void AlUSequence::_beatVideoClips(int64_t timeInUS) {
    auto clips = std::make_shared<AlVector<std::shared_ptr<AlMediaClip>>>();
    _findClipsByTime(AlMediaTrack::kType::VIDEO, *clips, timeInUS);
    if (!clips->empty()) {
        auto *msg1 = AlMessage::obtain(MSG_SEQUENCE_BEAT_VIDEO);
        msg1->arg2 = timeInUS;
        msg1->sp = clips;
        postMessage(msg1);
    }
}
