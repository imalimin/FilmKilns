/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwAudioInput.h"
#include "TimeUtils.h"
#include "Thread.h"

HwAudioInput::HwAudioInput() : HwAudioInput(nullptr) {
}

HwAudioInput::HwAudioInput(HandlerThread *handlerThread) : HwStreamMedia(handlerThread) {
    name = __FUNCTION__;
    registerEvent(EVENT_COMMON_PREPARE, reinterpret_cast<EventFunc>(&HwAudioInput::eventPrepare));
    registerEvent(EVENT_AUDIO_START, reinterpret_cast<EventFunc>(&HwAudioInput::eventStart));
    registerEvent(EVENT_AUDIO_PAUSE, reinterpret_cast<EventFunc>(&HwAudioInput::eventPause));
    registerEvent(EVENT_AUDIO_STOP, reinterpret_cast<EventFunc>(&HwAudioInput::eventStop));
    registerEvent(EVENT_AUDIO_SEEK, reinterpret_cast<EventFunc>(&HwAudioInput::eventSeek));
    registerEvent(EVENT_AUDIO_LOOP, reinterpret_cast<EventFunc>(&HwAudioInput::eventLoop));
    registerEvent(EVENT_AUDIO_SET_SOURCE,
                  reinterpret_cast<EventFunc>(&HwAudioInput::eventSetSource));
    decoder = new AsynAudioDecoder();
}

HwAudioInput::~HwAudioInput() {
    LOGI("HwAudioInput::~HwAudioInput");
    simpleLock.lock();
    if (decoder) {
        delete decoder;
        decoder = nullptr;
    }
    simpleLock.unlock();
}

bool HwAudioInput::eventPrepare(Message *msg) {
    playState = PAUSE;
    if (!decoder->prepare(path)) {
        LOGE("HwAudioInput::open %s failed", path.c_str());
    }
    return false;
}

bool HwAudioInput::eventRelease(Message *msg) {
    LOGI("HwAudioInput::eventRelease");
    eventStop(nullptr);
    return false;
}

bool HwAudioInput::eventSetSource(Message *msg) {
    string *str = static_cast<string *>(msg->tyrUnBox());
    this->path = string(str->c_str());
    delete msg->tyrUnBox();
    return false;
}

bool HwAudioInput::eventStart(Message *msg) {
    LOGI("HwAudioInput::eventStart");
    if (PAUSE == playState) {
        playState = PLAYING;
        if (decoder) {
            decoder->start();
        }
        loop();
    }
    return false;
}

bool HwAudioInput::eventPause(Message *msg) {
    if (PLAYING == playState) {
        playState = PAUSE;
        if (decoder) {
            decoder->pause();
        }
    }
    return false;
}

bool HwAudioInput::eventStop(Message *msg) {
    if (STOP != playState) {
        playState = STOP;
        if (decoder) {
            decoder->stop();
        }
    }
    return false;
}

bool HwAudioInput::eventSeek(Message *msg) {
    int64_t us = msg->arg2;
    decoder->seek(us);
    return true;
}

bool HwAudioInput::eventLoop(Message *msg) {
    if (PLAYING != playState) {
        return false;
    }
    simpleLock.lock();
    HwResult ret = grab();
    simpleLock.unlock();
    if (Hw::MEDIA_EOF == ret) {
        Logcat::i("HWVC", "HwAudioInput::eventLoop EOF");
        if (enableLoop) {
            decoder->seek(0);
            decoder->start();
            Logcat::i("HWVC", "HwAudioInput::eventLoop play loop.");
            loop();
        } else {
            eventPause(nullptr);
        }
        return true;
    }
    loop();
    return true;
}

void HwAudioInput::loop() {
    postEvent(new Message(EVENT_AUDIO_LOOP, nullptr));
}

HwResult HwAudioInput::grab() {
    HwAbsMediaFrame *frame = nullptr;
    HwResult ret = decoder->grab(&frame);
    if (Hw::MEDIA_WAIT == ret) {
        Logcat::i("HWVC", "HwAudioInput::grab wait");
        Thread::sleep(5000);
    }
    if (!frame) {
        return ret;
    }
    if (frame->isAudio()) {
        playFrame(dynamic_cast<HwAudioFrame *>(frame->clone()));
        processPlayListener(frame->getPts());
    }
    return ret;
}

void HwAudioInput::playFrame(HwAudioFrame *frame) {
    Message *msg = new Message(EVENT_SPEAKER_FEED, nullptr);
    msg->obj = frame;
    postEvent(msg);
}

void HwAudioInput::processPlayListener(int64_t us) {
    if (playListener) {
        if (llabs(lastPlayPts - us) >= INTERVAL_PROGRESS) {
            int64_t time = getCurrentTimeUS();
            playListener(us, decoder->getAudioDuration());
            Logcat::i("HWVC", "HwVideoInput::play callback cost %lld",
                      getCurrentTimeUS() - time);
            lastPlayPts = us;
        }
    }
}

void HwAudioInput::setPlayListener(function<void(int64_t, int64_t)> listener) {
    this->playListener = listener;
}