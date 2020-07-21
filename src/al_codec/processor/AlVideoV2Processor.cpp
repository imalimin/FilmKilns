/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlVideoV2Processor.h"

#include "AlUTimeline.h"
#include "AlUSequence.h"
#include "AlUAudios.h"
#include "AlUVideos.h"
#include "HwSpeaker.h"
#include "AlGImage.h"
#include "AlScreen.h"
#include "AlFuture.h"
#include "TimeUtils.h"

#define TAG "AlVideoV2Processor"

AlVideoV2Processor::AlVideoV2Processor() : AlAbsProcessor(TAG) {
    registerAnUnit(new AlScreen(ALIAS_SCREEN));
    registerAnUnit(new HwSpeaker(ALIAS_SPEAKER, HwAudioDeviceMode::LowLatency));
    registerAnUnit(new AlUTimeline(ALIAS_TIMELINE));
    registerAnUnit(new AlUSequence(ALIAS_SEQUENCE));
    registerAnUnit(new AlUAudios(ALIAS_AUDIOS));
    registerAnUnit(new AlUVideos(ALIAS_VIDEOS));
    registerAnUnit(new AlGImage(ALIAS_IMAGE));

    al_reg_msg(MSG_TIMELINE_PROGRESS_NOTIFY, AlVideoV2Processor::_onTimelineInUS);
    al_reg_msg(MSG_SEQUENCE_TRACK_ADD_DONE, AlVideoV2Processor::_onAddTrackDone);
    al_reg_msg(MSG_SEQUENCE_TRACK_REMOVE_DONE, AlVideoV2Processor::_onRemoveTrackDone);
    al_reg_msg(MSG_SEQUENCE_TRACK_UPDATE_NOTIFY, AlVideoV2Processor::_onTrackUpdate);
}

AlVideoV2Processor::~AlVideoV2Processor() {

}

void AlVideoV2Processor::onCreate() {
    AlAbsProcessor::onCreate();
    this->aBaseCtx = AlEgl::offScreen(TAG);
}

void AlVideoV2Processor::onDestroy() {
    AlAbsProcessor::onDestroy();
    delete this->aBaseCtx;
    this->aBaseCtx = nullptr;
}

bool AlVideoV2Processor::_onTimelineInUS(AlMessage *msg) {
    auto progress = msg->arg1;
    auto duration = msg->arg2;
    playProgressListener(progress * duration / 1e6, duration);
    return true;
}

bool AlVideoV2Processor::_onAddTrackDone(AlMessage *msg) {
    mCurTrackID = msg->arg1;
    return true;
}

bool AlVideoV2Processor::_onRemoveTrackDone(AlMessage *msg) {
    return true;
}

bool AlVideoV2Processor::_onTrackUpdate(AlMessage *msg) {
    if (trackUpdateListener) {
        auto track = std::static_pointer_cast<AlMediaTrack>(msg->sp);
        trackUpdateListener(track);
    }
    return true;
}

int32_t AlVideoV2Processor::addTrack(AlMediaTrack::kType type, std::string path,
                                     int64_t seqInInUS, int64_t seqOutInUS,
                                     int64_t trimInInUS, int64_t trimOutInUS) {
    AlFileDescriptor descriptor(std::move(path));
    auto clip = std::make_shared<AlMediaClip>(0, descriptor);
    clip->setSeqIn(seqInInUS);
    clip->setTrimIn(trimInInUS);

    auto bundle = std::make_shared<AlFuture>();
    bundle->sp = clip;

    AlMessage *msg = AlMessage::obtain(MSG_SEQUENCE_TRACK_ADD);
    msg->arg1 = (int32_t) type;
    msg->sp = bundle;
    postMessage(msg);
    return bundle->get(-1);
}

void AlVideoV2Processor::removeTrack(int32_t trackID) {
    AlMessage *msg = AlMessage::obtain(MSG_SEQUENCE_TRACK_REMOVE);
    msg->arg1 = trackID;
    postMessage(msg);
}

void AlVideoV2Processor::start() {
    postMessage(AlMessage::obtain(MSG_TIMELINE_START));
}

void AlVideoV2Processor::pause() {
    postMessage(AlMessage::obtain(MSG_TIMELINE_PAUSE));
}

void AlVideoV2Processor::seek(int64_t timeInUS) {
    auto *msg = AlMessage::obtain(MSG_TIMELINE_SEEK, AlMessage::QUEUE_MODE_UNIQUE);
    msg->arg2 = timeInUS;
    postMessage(msg);
}

void AlVideoV2Processor::setPlayProgressListener(OnPlayProgressListener l) {
    playProgressListener = std::move(l);
}

void AlVideoV2Processor::updateWindow(HwWindow *win) {
    AlMessage *msg = AlMessage::obtain(EVENT_SCREEN_UPDATE_WINDOW, new NativeWindow(win, nullptr));
    postEvent(msg);
    postEvent(AlMessage::obtain(EVENT_COMMON_INVALIDATE, AlMessage::QUEUE_MODE_UNIQUE));
}

void AlVideoV2Processor::setOnTrackUpdateListener(AlVideoV2Processor::OnTrackUpdateListener l) {
    trackUpdateListener = std::move(l);
}
