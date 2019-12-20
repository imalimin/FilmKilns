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

HwVideoInput::HwVideoInput(string alias) : HwStreamMedia(alias) {
    registerEvent(EVENT_VIDEO_START, reinterpret_cast<EventFunc>(&HwVideoInput::eventStart));
    registerEvent(EVENT_VIDEO_PAUSE, reinterpret_cast<EventFunc>(&HwVideoInput::eventPause));
    registerEvent(EVENT_VIDEO_SEEK, reinterpret_cast<EventFunc>(&HwVideoInput::eventSeek));
    registerEvent(EVENT_VIDEO_SET_SOURCE,
                  reinterpret_cast<EventFunc>(&HwVideoInput::eventSetSource));
    registerEvent(EVENT_VIDEO_LOOP, reinterpret_cast<EventFunc>(&HwVideoInput::eventLoop));
    registerEvent(EVENT_VIDEO_STOP, reinterpret_cast<EventFunc>(&HwVideoInput::eventStop));
    decoder = new AsynVideoDecoder();
}

HwVideoInput::~HwVideoInput() {
    LOGI("HwVideoInput::HwVideoInputeoInput");
    simpleLock.lock();
    if (decoder) {
        delete decoder;
        decoder = nullptr;
    }
    simpleLock.unlock();
    playListener = nullptr;
}

bool HwVideoInput::onDestroy(AlMessage *msg) {
    LOGI("HwVideoInput::onDestroy");
    eventStop(nullptr);
    if (yuvFilter) {
        delete yuvFilter;
        yuvFilter = nullptr;
    }
    if (texAllocator) {
        delete texAllocator;
        texAllocator = nullptr;
    }
    return true;
}

bool HwVideoInput::onCreate(AlMessage *msg) {
    playState = PAUSE;
    if (!decoder->prepare(path)) {
        LOGE("HwVideoInput::open %s failed", path.c_str());
        eventStop(nullptr);
        return true;
    }
    if (!texAllocator) {
        texAllocator = new AlTexAllocator();
    }
    return true;
}

bool HwVideoInput::eventStart(AlMessage *msg) {
    LOGI("HwVideoInput::eventStart");
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
    playState = STOP;
    Logcat::i("HWVC", "HwVideoInput::eventStop");
    return true;
}

bool HwVideoInput::eventSetSource(AlMessage *msg) {
    string *str = static_cast<string *>(msg->getObj<ObjectBox *>()->ptr);
    this->path = string(str->c_str());
    delete str;
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
        Logcat::i("HWVC", "HwVideoInput::eventLoop EOF");
        if (enableLoop) {
            decoder->seek(0);
            decoder->start();
            Logcat::i("HWVC", "HwVideoInput::eventLoop play loop.");
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
        AlTexDescription desc;
        target = HwTexture::alloc(desc);
        target->update(nullptr, w, h, GL_RGBA);
    }
}

void HwVideoInput::bindTex(HwVideoFrame *frame) {
    if (!y || !u || !v) {
        y = texAllocator->alloc();
        u = texAllocator->alloc();
        v = texAllocator->alloc();
    }
    int size = frame->getWidth() * frame->getHeight();
    HwBuffer *buf = HwBuffer::wrap(frame->data(), size);
    y->update(buf, frame->getWidth(), frame->getHeight(), GL_LUMINANCE);
    delete buf;

    buf = HwBuffer::wrap(frame->data() + size, size / 4);
    u->update(buf, frame->getWidth() / 2, frame->getHeight() / 2, GL_LUMINANCE);
    delete buf;
    buf = HwBuffer::wrap(frame->data() + size + size / 4, size / 4);
    v->update(buf, frame->getWidth() / 2, frame->getHeight() / 2, GL_LUMINANCE);
    delete buf;
}

HwResult HwVideoInput::grab() {
    int64_t time = getCurrentTimeUS();
    HwAbsMediaFrame *frame = nullptr;
    HwResult ret = decoder->grab(&frame);
    Logcat::i("HWVC", "HwVideoInput::grab cost: %lld, ret: %d", getCurrentTimeUS() - time,
              ret.code);
    if (!frame) {
        Logcat::i("HWVC", "HwVideoInput::grab wait");
        Thread::sleep(5000);
        return ret;
    }

    if (frame->isVideo()) {
        Logcat::i("HWVC", "HwVideoInput::play picture pts=%lld", frame->getPts());
        HwVideoFrame *videoFrame = dynamic_cast<HwVideoFrame *>(frame);
        int64_t curPts = frame->getPts();
        int64_t curTimeInUs = getCurrentTimeUS();
        if (lastPts >= 0 || lastShowTime >= 0) {
            int64_t delta = (curPts - lastPts) - (curTimeInUs - lastShowTime);
            if (delta > 0) { // @TODO To avoid waiting too long when seeking.
                Thread::sleep(delta);
            }
            LOGI("HwVideoInput::grab sleep %d x %d, delta time: %lld",
                 videoFrame->getWidth(),
                 videoFrame->getHeight(),
                 delta);
        }
        lastPts = curPts;
        lastShowTime = curTimeInUs;
        checkEnv(videoFrame->getWidth(), videoFrame->getHeight());
        bindTex(videoFrame);
//        lock->lock();
        glViewport(0, 0, videoFrame->getWidth(), videoFrame->getHeight());
//        lock->unlock();
        yuvFilter->draw(y, u, v, target);
        invalidate(target);
    } else if (frame->isAudio()) {
        playAudioFrame(dynamic_cast<HwAudioFrame *>(frame->clone()));
        Logcat::i("HWVC", "HwVideoInput::play audio pts=%lld", frame->getPts());
    }
    processPlayListener(frame->getPts());
    return ret;
}

bool HwVideoInput::invalidate(HwAbsTexture *tex) {
    AlMessage *msg = AlMessage::obtain(EVENT_RENDER_FILTER);
    msg->obj = HwTexture::wrap(dynamic_cast<HwTexture *>(tex));
    msg->desc = "RENDER";
    postEvent(msg);
    return true;
}

void HwVideoInput::playAudioFrame(HwAudioFrame *frame) {
    AlMessage *msg = AlMessage::obtain(EVENT_SPEAKER_FEED);
    msg->obj = frame;
    postEvent(msg);
}

void HwVideoInput::processPlayListener(int64_t us) {
    if (playListener) {
        if (llabs(lastPlayPts - us) >= INTERVAL_PROGRESS) {
            int64_t time = getCurrentTimeUS();
            playListener(us, decoder->getDuration());
            Logcat::i("HWVC", "HwVideoInput::play callback cost %lld",
                      getCurrentTimeUS() - time);
            lastPlayPts = us;
        }
    }
}

void HwVideoInput::setPlayListener(function<void(int64_t, int64_t)> listener) {
    this->playListener = listener;
}