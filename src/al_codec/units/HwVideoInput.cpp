/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/
#include "../include/HwVideoInput.h"
#include "ObjectBox.h"
#include "Size.h"
#include "TimeUtils.h"
#include "../include/HwVideoFrame.h"
#include "Thread.h"
#include "HwTexture.h"
#include "AlTexManager.h"

#define TAG "HwVideoInput"

HwVideoInput::HwVideoInput(string alias) : HwStreamMedia(alias) {
    registerEvent(EVENT_VIDEO_START, reinterpret_cast<EventFunc>(&HwVideoInput::eventStart));
    registerEvent(EVENT_VIDEO_PAUSE, reinterpret_cast<EventFunc>(&HwVideoInput::eventPause));
    registerEvent(EVENT_VIDEO_SEEK, reinterpret_cast<EventFunc>(&HwVideoInput::eventSeek));
    registerEvent(EVENT_VIDEO_SET_SOURCE,
                  reinterpret_cast<EventFunc>(&HwVideoInput::eventSetSource));
    registerEvent(EVENT_VIDEO_LOOP, reinterpret_cast<EventFunc>(&HwVideoInput::eventLoop));
    registerEvent(EVENT_VIDEO_STOP, reinterpret_cast<EventFunc>(&HwVideoInput::eventStop));
    registerEvent(EVENT_LAYER_QUERY_ID_NOTIFY,
                  reinterpret_cast<EventFunc>(&HwVideoInput::_onLayerNotify));
    decoder = new AsynVideoDecoder();
}

HwVideoInput::~HwVideoInput() {
    simpleLock.lock();
    if (decoder) {
        delete decoder;
        decoder = nullptr;
    }
    simpleLock.unlock();
}

bool HwVideoInput::onDestroy(AlMessage *msg) {
    AlLogI(TAG, "");
    eventStop(nullptr);
    if (yuvFilter) {
        delete yuvFilter;
        yuvFilter = nullptr;
    }
    AlTexManager::instance()->recycle(&y);
    AlTexManager::instance()->recycle(&u);
    AlTexManager::instance()->recycle(&v);
    return true;
}

bool HwVideoInput::onCreate(AlMessage *msg) {
    AlLogI(TAG, "");
    playState = PAUSE;
    return true;
}

bool HwVideoInput::eventStart(AlMessage *msg) {
    AlLogI(TAG, "");
    if (PAUSE == playState) {
        playState = PLAYING;
        if (decoder) {
            decoder->start();
        }
        loop();
    }
    return true;
}

bool HwVideoInput::eventPause(AlMessage *msg) {
    if (STOP != playState) {
        playState = PAUSE;
    }
    return true;
}

bool HwVideoInput::eventSeek(AlMessage *msg) {
    int64_t us = msg->arg2;
    lastPts = -1;
    lastShowTime = -1;
    decoder->seek(us);
    return true;
}

bool HwVideoInput::eventStop(AlMessage *msg) {
    AlLogI(TAG, "");
    playState = STOP;
    return true;
}

bool HwVideoInput::eventSetSource(AlMessage *msg) {
    this->path = msg->desc;
    if (!decoder->prepare(path)) {
        AlLogE(TAG, "open file(%s) failed", path.c_str());
        eventStop(nullptr);
        return true;
    }
    auto *m = AlMessage::obtain(MSG_LAYER_ADD_EMPTY,
                                new AlSize(decoder->width(), decoder->height()));
    postMessage(m);
    return true;
}

void HwVideoInput::loop() {
    postEvent(AlMessage::obtain(EVENT_VIDEO_LOOP, nullptr, Message::QUEUE_MODE_UNIQUE));
}

bool HwVideoInput::eventLoop(AlMessage *msg) {
    if (PLAYING != playState) {
        return true;
    }
    simpleLock.lock();
    HwResult ret = grab();
    simpleLock.unlock();
    if (Hw::MEDIA_EOF == ret) {
        AlLogI(TAG, " eof");
        if (enableLoop) {
            decoder->seek(0);
            decoder->start();
            AlLogI(TAG, "play loop.");
            loop();
        } else {
            eventPause(nullptr);
        }
        return true;
    }
    loop();
    return true;
}

void HwVideoInput::checkEnv(int32_t w, int32_t h) {
    if (!yuvFilter) {
        yuvFilter = new HwYV122RGBAFilter();
        yuvFilter->prepare();
    }
}

void HwVideoInput::bindTex(HwVideoFrame *frame) {
    if (!y || !u || !v) {
        y = AlTexManager::instance()->alloc();
        u = AlTexManager::instance()->alloc();
        v = AlTexManager::instance()->alloc();
    }
    int size = frame->getWidth() * frame->getHeight();
    AlBuffer *buf = AlBuffer::wrap(frame->data(), size);
    y->update(buf, frame->getWidth(), frame->getHeight(), GL_LUMINANCE);
    delete buf;

    buf = AlBuffer::wrap(frame->data() + size, size / 4);
    u->update(buf, frame->getWidth() / 2, frame->getHeight() / 2, GL_LUMINANCE);
    delete buf;
    buf = AlBuffer::wrap(frame->data() + size + size / 4, size / 4);
    v->update(buf, frame->getWidth() / 2, frame->getHeight() / 2, GL_LUMINANCE);
    delete buf;
}

HwResult HwVideoInput::grab() {
    int64_t time = getCurrentTimeUS();
    HwAbsMediaFrame *frame = nullptr;
    HwResult ret = decoder->grab(&frame);
//    AlLogI(TAG, "cost: %lld, ret: %d", getCurrentTimeUS() - time, ret.code);
    if (!frame) {
//        AlLogI(TAG, "wait");
        Thread::sleep(5000);
        return ret;
    }

    if (frame->isVideo()) {
//        AlLogI(TAG, "play picture pts=%lld", frame->getPts());
        HwVideoFrame *videoFrame = dynamic_cast<HwVideoFrame *>(frame);
        int64_t curPts = frame->getPts();
        int64_t curTimeInUs = getCurrentTimeUS();
        if (lastPts >= 0 || lastShowTime >= 0) {
            int64_t delta = (curPts - lastPts) - (curTimeInUs - lastShowTime);
            if (delta > 0) { // @TODO To avoid waiting too long when seeking.
                Thread::sleep(delta);
            }
//            AlLogI(TAG, "sleep %d x %d, delta time: %"
//                    PRId64,
//                   videoFrame->getWidth(),
//                   videoFrame->getHeight(),
//                   delta);
        }
        lastPts = curPts;
        lastShowTime = curTimeInUs;
        checkEnv(videoFrame->getWidth(), videoFrame->getHeight());
        bindTex(videoFrame);
//        lock->lock();
        glViewport(0, 0, videoFrame->getWidth(), videoFrame->getHeight());
//        lock->unlock();
        yuvFilter->draw(y, u, v, mLayerTex);
        _invalidate();
    } else if (frame->isAudio()) {
        playAudioFrame(dynamic_cast<HwAudioFrame *>(frame->clone()));
//        AlLogI(TAG, "play audio pts=%lld", frame->getPts());
    }
    _notifyProgress(frame->getPts());
    return ret;
}

bool HwVideoInput::_invalidate() {
    AlMessage *msg = AlMessage::obtain(EVENT_COMMON_INVALIDATE, AlMessage::QUEUE_MODE_UNIQUE);
    msg->arg1 = 0;
    postMessage(msg);
    return true;
}

void HwVideoInput::playAudioFrame(HwAudioFrame *frame) {
    AlMessage *msg = AlMessage::obtain(EVENT_SPEAKER_FEED);
    msg->obj = frame;
    postEvent(msg);
}

void HwVideoInput::_notifyProgress(int64_t us) {
    if (llabs(lastPlayPts - us) >= INTERVAL_PROGRESS) {
        int64_t time = getCurrentTimeUS();
//        AlLogI(TAG, "play callback cost %"
//                PRId64, getCurrentTimeUS() - time);
        lastPlayPts = us;

        AlMessage *msg = AlMessage::obtain(MSG_VIDEO_PROGRESS);
        msg->arg1 = us;
        msg->arg2 = decoder->getDuration();
        postMessage(msg);
    }
}

bool HwVideoInput::_onLayerNotify(AlMessage *msg) {
    mLayerId = msg->arg1;
    if (nullptr == mLayerTex) {
        mLayerTex = HwTexture::wrap(msg->getObj<HwAbsTexture *>());
    }
    AlLogI(TAG, "%d", mLayerId);
    return true;
}
