/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlUTimeline.h"
#include "Thread.h"
#include "TimeUtils.h"

#define TAG "AlUTimeline"

AlUTimeline::AlUTimeline(const std::string alias) : Unit(alias) {
    al_reg_msg(MSG_TIMELINE_SET_HZ_IN_US, AlUTimeline::_onSetHzInUS);
    al_reg_msg(MSG_TIMELINE_SET_DURATION, AlUTimeline::_onSetDurationUS);
    pipe = std::shared_ptr<AlEventPipeline>(AlEventPipeline::create(TAG));
}

AlUTimeline::~AlUTimeline() {

}

bool AlUTimeline::onCreate(AlMessage *msg) {
    _heartbeat();
    return true;
}

bool AlUTimeline::onDestroy(AlMessage *msg) {
    return true;
}

bool AlUTimeline::_onSetHzInUS(AlMessage *msg) {
//    hzInUS = msg->arg1;
    hzInUS = 23219;
    return true;
}

bool AlUTimeline::_onSetDurationUS(AlMessage *msg) {
    mDurationInUS = msg->arg2;
    AlLogI(TAG, "%" PRId64, mDurationInUS);
    return true;
}

void AlUTimeline::_heartbeat() {
    if (mDurationInUS > 0) {
        auto *msg = AlMessage::obtain(MSG_TIMELINE_PROGRESS_NOTIFY, AlMessage::QUEUE_MODE_UNIQUE);
        msg->arg1 = mCurTimeInUS * 1e6 / mDurationInUS;
        msg->arg2 = mDurationInUS;
        this->postMessage(msg);
    }

    pipe->queueEvent([this]() {
        this->_sendBeat();
        auto time = TimeUtils::getCurrentTimeUS();
        auto sleepTime = this->mLastBeatTimestampInUS > 0 ? (this->hzInUS -
                                                             (time - this->mLastBeatTimestampInUS))
                                                          : this->hzInUS;
        this->mLastBeatTimestampInUS = time;
        AlEventPipeline::sleep(sleepTime);
        this->_heartbeat();
    });
}

void AlUTimeline::_sendBeat() {
    // 如果QUEUE_MODE_FIRST_ALWAYS会影响数据流传递，所以使用QUEUE_MODE_UNIQUE即可
    auto mode = AlMessage::QUEUE_MODE_UNIQUE;
    auto *msg = AlMessage::obtain(MSG_TIMELINE_HEARTBEAT, mode);
    msg->arg2 = this->mCurTimeInUS;
    this->postMessage(msg);
    this->mCurTimeInUS += this->hzInUS;
    if (this->mCurTimeInUS > this->mDurationInUS) {
        this->mCurTimeInUS = 0;
    }
}
