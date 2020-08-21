/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlUTimeline.h"
#include "Thread.h"
#include "TimeUtils.h"
#include "AlFuture.h"

#define TAG "AlUTimeline"

AlUTimeline::AlUTimeline(const std::string alias) : Unit(alias) {
    al_reg_msg(MSG_TIMELINE_SET_HZ_IN_US, AlUTimeline::_onSetHzInUS);
    al_reg_msg(MSG_TIMELINE_SET_DURATION, AlUTimeline::_onSetDurationUS);
    al_reg_msg(MSG_TIMELINE_START, AlUTimeline::_onStart);
    al_reg_msg(MSG_TIMELINE_PAUSE, AlUTimeline::_onPause);
    al_reg_msg(MSG_TIMELINE_SEEK, AlUTimeline::_onSeek);
    al_reg_msg(MSG_TIMELINE_GET_STATUS, AlUTimeline::_onGetStatus);
    pipe = std::shared_ptr<AlEventPipeline>(AlEventPipeline::create(TAG));
}

AlUTimeline::~AlUTimeline() {

}

bool AlUTimeline::onCreate(AlMessage *msg) {
    return true;
}

bool AlUTimeline::onDestroy(AlMessage *msg) {
    return true;
}

bool AlUTimeline::_onSetHzInUS(AlMessage *msg) {
    durationOfBeatInNS = 1024 *1e9 / 44100 / 4;
    return true;
}

bool AlUTimeline::_onSetDurationUS(AlMessage *msg) {
    mDurationInUS = msg->arg2;
    AlLogI(TAG, "%" PRId64, mDurationInUS);
    return true;
}

bool AlUTimeline::_onStart(AlMessage *msg) {
    mClockStartTime = TimeUtils::getCurrentTimeUS();
    beating = true;
    _heartbeat();
    return true;
}

bool AlUTimeline::_onPause(AlMessage *msg) {
    mClockTime += TimeUtils::getCurrentTimeUS() - mClockStartTime;
    beating = false;
    return true;
}

bool AlUTimeline::_onSeek(AlMessage *msg) {
    this->mClockTime = std::max<int64_t>(msg->arg2, 0);
    this->mClockTime = std::min<int64_t>(mDurationInUS, this->mClockTime);
    auto *msg1 = AlMessage::obtain(MSG_TIMELINE_SEEK_NOTIFY, AlMessage::QUEUE_MODE_UNIQUE);
    msg1->arg2 = this->mClockTime;
    postMessage(msg1);
    return true;
}

bool AlUTimeline::_onGetStatus(AlMessage *msg) {
    auto bundle = std::static_pointer_cast<AlFuture>(msg->sp);
    bundle->put(beating);
    return true;
}

void AlUTimeline::_heartbeat() {
    if (!beating) {
        return;
    }
    if (mDurationInUS > 0) {
        auto *msg = AlMessage::obtain(MSG_TIMELINE_PROGRESS_NOTIFY, AlMessage::QUEUE_MODE_UNIQUE);
        msg->arg1 = mCurTimeInUS * 1e6 / mDurationInUS;
        msg->arg2 = mDurationInUS;
        this->postMessage(msg);
    }

    pipe->queueEvent([this]() {
        this->_sendBeat();
        this_thread::sleep_for(chrono::nanoseconds(durationOfBeatInNS));
        this->_heartbeat();
    });
}

void AlUTimeline::_sendBeat() {
    if (0 == this->mCurTimeInUS) {
        postMessage(AlMessage::obtain(MSG_TIMELINE_BEGIN, AlMessage::QUEUE_MODE_UNIQUE));
    }
    // 如果QUEUE_MODE_FIRST_ALWAYS会影响数据流传递，所以使用QUEUE_MODE_UNIQUE即可
    auto *msg = AlMessage::obtain(MSG_TIMELINE_HEARTBEAT, AlMessage::QUEUE_MODE_UNIQUE);
    msg->arg2 = this->mCurTimeInUS;
    this->postMessage(msg);
    this->mCurTimeInUS = mClockTime + TimeUtils::getCurrentTimeUS() - mClockStartTime;
    AlLogD(TAG, "%" PRId64 ", %" PRId64, mCurTimeInUS, mDurationInUS);
    if (this->mCurTimeInUS > this->mDurationInUS) {
        auto *msg1 = AlMessage::obtain(MSG_TIMELINE_END, AlMessage::QUEUE_MODE_UNIQUE);
        msg1->arg2 = this->mDurationInUS;
        postMessage(msg1);
        this->mCurTimeInUS = 0;
        this->mClockStartTime = TimeUtils::getCurrentTimeUS();
        this->mClockTime = 0;
    }
}
