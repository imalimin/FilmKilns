/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlMediaTrack.h"
#include "AlLogcat.h"
#include "AlMath.h"

#define TAG "AlMediaTrack"

AlMediaTrack::AlMediaTrack(AlID id, AlMediaTrack::kType type)
        : Object(), _id(id), _type(type) {
}

AlMediaTrack::AlMediaTrack(const AlMediaTrack &o)
        : Object(), _id(o._id), _type(o._type) {
}

AlMediaTrack::~AlMediaTrack() {
    clips.clear();
    AlLogI(TAG, "");
}

AlID AlMediaTrack::id() {
    return _id;
}

AlMediaTrack::kType AlMediaTrack::type() {
    return _type;
}

int64_t AlMediaTrack::getSeqIn() {
    int64_t timeInUS = 0;
    auto itr = clips.begin();
    while (clips.end() != itr) {
        timeInUS = std::max(timeInUS, itr->second->getSeqIn());
        ++itr;
    }
    return timeInUS;
}

int64_t AlMediaTrack::getSeqOut() {
    int64_t timeInUS = 0;
    auto itr = clips.begin();
    while (clips.end() != itr) {
        timeInUS = std::max(timeInUS, itr->second->getSeqOut());
        ++itr;
    }
    return timeInUS;
}

int64_t AlMediaTrack::getDuration() {
    int64_t timeInUS = 0;
    for (auto &clip : clips) {
        timeInUS += clip.second->getDuration();
    }
    return timeInUS;
}

int64_t AlMediaTrack::getMinFrameDuration() {
    int64_t timeInUS = INT64_MAX;
    for (auto &clip : clips) {
        timeInUS = std::min<int64_t>(clip.second->getFrameDuration(), timeInUS);
    }
    return timeInUS;
}

AlID AlMediaTrack::addClip(AlID id, const AlFileDescriptor &o) {
    auto itr = clips.find(id);
    if (clips.end() != itr) {
        AlLogW(TAG, "Clip(id=%d) existed.", id);
        return AlIdentityCreator::NONE_ID;
    }
    auto clip = std::make_unique<AlMediaClip>(id, o);
    clips.emplace(std::make_pair(id, std::move(clip)));
    return id;
}

AlMediaClip *AlMediaTrack::findClip(AlID id) {
    auto itr = clips.find(id);
    if (clips.end() == itr) {
        return nullptr;
    }
    return itr->second.get();
}

size_t AlMediaTrack::findClips(AlVector<std::shared_ptr<AlMediaClip>> &array, int64_t timeInUS) {
    size_t count = 0;
    auto itr = clips.begin();
    while (clips.end() != itr) {
        if (timeInUS >= itr->second->getSeqIn() && timeInUS < itr->second->getSeqOut()) {
            array.push_back(std::make_shared<AlMediaClip>(*itr->second));
            ++count;
        }
        ++itr;
    }
    return count;
}

size_t AlMediaTrack::findAllClips(AlVector<std::shared_ptr<AlMediaClip>> &array) {
    size_t count = 0;
    for (auto &clip : this->clips) {
        array.push_back(std::make_shared<AlMediaClip>(*clip.second));
        ++count;
    }
    return count;
}
