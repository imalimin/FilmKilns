/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlUTimeline.h"
#include "Thread.h"

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
    hzInUS = msg->arg1;
    return true;
}

bool AlUTimeline::_onSetDurationUS(AlMessage *msg) {
    mDurationInUS = msg->arg2;
    AlLogI(TAG, "%" PRId64, mDurationInUS);
    return true;
}

void AlUTimeline::_heartbeat() {
    pipe->queueEvent([this]() {
        auto *msg = AlMessage::obtain(MSG_TIMELINE_HEARTBEAT, AlMessage::QUEUE_MODE_UNIQUE);
        msg->arg2 = this->mCurTimeInUS;
        this->postMessage(msg);
        this->mCurTimeInUS += this->hzInUS;
        AlEventPipeline::sleep(this->hzInUS);
        this->_heartbeat();
    });
}
