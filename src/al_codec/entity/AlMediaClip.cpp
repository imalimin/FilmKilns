/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlMediaClip.h"
#include "AlLogcat.h"

#define TAG "AlMediaClip"

AlMediaClip::AlMediaClip(int32_t id, const AlAbsInputDescriptor &o)
        : Object(),
          _id(id),
          iDescriptor(std::move(std::make_shared<AlAbsInputDescriptor>(o))) {

}

AlMediaClip::AlMediaClip(const AlMediaClip &o)
        : Object(),
          _id(o._id),
          seqIn(o.seqIn),
          trimIn(o.trimIn),
          duration(o.duration),
          iDescriptor(std::move(std::make_shared<AlAbsInputDescriptor>(*o.iDescriptor))) {

}

AlMediaClip::~AlMediaClip() {
    AlLogI(TAG, "");
}

int32_t AlMediaClip::id() {
    return _id;
}

void AlMediaClip::setSeqIn(int64_t timeInUS) {
    seqIn = timeInUS;
}

int64_t AlMediaClip::getSeqIn() {
    return seqIn;
}

int64_t AlMediaClip::getSeqOut() {
    return seqIn + duration;
}

void AlMediaClip::setTrimIn(int64_t timeInUS) {
    trimIn = timeInUS;
}

int64_t AlMediaClip::getTrimIn() {
    return trimIn;
}

int64_t AlMediaClip::getTrimOut() {
    return trimIn + duration;
}

void AlMediaClip::setDuration(int64_t timeInUS) {
    duration = timeInUS;
}

int64_t AlMediaClip::getDuration() {
    return duration;
}