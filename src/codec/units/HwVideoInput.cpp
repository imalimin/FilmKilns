/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/
#include "../include/HwVideoInput.h"
#include "ObjectBox.h"
#include "Size.h"
#include "TimeUtils.h"
#include "../include/HwVideoFrame.h"
#include "Thread.h"

HwVideoInput::HwVideoInput() : HwVideoInput(nullptr) {
}

HwVideoInput::HwVideoInput(HandlerThread *handlerThread) : HwStreamMedia(handlerThread) {
    name = __FUNCTION__;
    registerEvent(EVENT_COMMON_PREPARE, reinterpret_cast<EventFunc>(&HwVideoInput::eventPrepare));
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
    if (path) {
        delete[]path;
        path = nullptr;
    }
    simpleLock.unlock();
    playListener = nullptr;
}

bool HwVideoInput::eventRelease(Message *msg) {
    LOGI("HwVideoInput::eventRelease");
    eventStop(nullptr);
    if (texAllocator) {
        delete texAllocator;
        texAllocator = nullptr;
    }
    return true;
}

bool HwVideoInput::eventPrepare(Message *msg) {
    playState = PAUSE;
    if (!decoder->prepare(path)) {
        LOGE("HwVideoInput::open %s failed", path);
        return true;
    }
    if (!texAllocator) {
        texAllocator = new TextureAllocator();
    }
    return true;
}

bool HwVideoInput::eventStart(Message *msg) {
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

bool HwVideoInput::eventPause(Message *msg) {
    if (STOP != playState) {
        playState = PAUSE;
    }
    if (decoder) {
        decoder->pause();
    }
    return true;
}

bool HwVideoInput::eventSeek(Message *msg) {
    int64_t us = msg->arg2;
    decoder->seek(us);
    return true;
}

bool HwVideoInput::eventStop(Message *msg) {
    playState = STOP;
    Logcat::i("HWVC", "HwVideoInput::eventStop");
    return true;
}

bool HwVideoInput::eventSetSource(Message *msg) {
    this->path = static_cast<char *>(msg->tyrUnBox());
    return true;
}

void HwVideoInput::loop() {
    postEvent(new Message(EVENT_VIDEO_LOOP, nullptr));
}

bool HwVideoInput::eventLoop(Message *msg) {
    if (PLAYING != playState) {
        return true;
    }
    simpleLock.lock();
    int ret = grab();
    simpleLock.unlock();
    if (MEDIA_TYPE_EOF == ret) {
        eventStop(nullptr);
        return true;
    }
    loop();
    return true;
}

void HwVideoInput::checkFilter() {
    if (!yuvFilter) {
        yuvFilter = new YUV420PFilter();
        yuvFilter->init(decoder->width(), decoder->height());
        yuv[0] = texAllocator->alloc();
        yuv[1] = texAllocator->alloc();
        yuv[2] = texAllocator->alloc();
    }
}

int HwVideoInput::grab() {
    int64_t time = getCurrentTimeUS();
    HwAbsMediaFrame *frame = nullptr;
    int ret = decoder->grab(&frame);
    Logcat::i("HWVC", "HwVideoInput::grab cost: %lld, ret: %d", getCurrentTimeUS() - time, ret);
    if (!frame) {
        Logcat::i("HWVC", "HwVideoInput::grab wait");
        Thread::sleep(5000);
        return ret;
    }

    if (frame->isVideo()) {
        Logcat::i("HWVC", "HwVideoInput::play picture pts=%lld", frame->getPts());
        HwVideoFrame *videoFrame = dynamic_cast<HwVideoFrame *>(frame);
        lastShowTime = getCurrentTimeUS();
        int64_t curPts = frame->getPts();
        if (lastPts > 0) {
            int64_t t = (curPts - lastPts) - (getCurrentTimeUS() - lastShowTime);
//            if (t < 60000) { // @TODO To avoid waiting too long when seeking.
//                Thread::sleep(t);
//            }
            LOGI("HwVideoInput::grab %d x %d, delta time: %lld, wait time: %lld",
                 videoFrame->getWidth(),
                 videoFrame->getHeight(),
                 (getCurrentTimeUS() - lastShowTime) / 1000, t);
        }
        lastPts = curPts;
        checkFilter();
        int size = videoFrame->getWidth() * videoFrame->getHeight();
        glBindTexture(GL_TEXTURE_2D, yuv[0]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, videoFrame->getWidth(),
                     videoFrame->getHeight(), 0,
                     GL_LUMINANCE,
                     GL_UNSIGNED_BYTE,
                     frame->getBuffer()->getData());
        glBindTexture(GL_TEXTURE_2D, yuv[1]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, videoFrame->getWidth() / 2,
                     videoFrame->getHeight() / 2, 0,
                     GL_LUMINANCE,
                     GL_UNSIGNED_BYTE,
                     frame->getBuffer()->getData() + size);
        glBindTexture(GL_TEXTURE_2D, yuv[2]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, videoFrame->getWidth() / 2,
                     videoFrame->getHeight() / 2, 0,
                     GL_LUMINANCE,
                     GL_UNSIGNED_BYTE,
                     frame->getBuffer()->getData() + size + size / 4);
        glBindTexture(GL_TEXTURE_2D, GL_NONE);

//        lock->lock();
        glViewport(0, 0, videoFrame->getWidth(), videoFrame->getHeight());
//        lock->unlock();
        yuvFilter->draw(yuv[0], yuv[1], yuv[2]);
        invalidate(yuvFilter->getFrameBuffer()->getFrameTexture(), videoFrame->getWidth(),
                   videoFrame->getHeight());
        return MEDIA_TYPE_VIDEO;
    } else if (frame->isAudio()) {
        HwAudioFrame *audioFrame = dynamic_cast<HwAudioFrame *>(frame);
        playAudioFrame(dynamic_cast<HwAudioFrame *>(frame->clone()));
        processPlayListener(audioFrame->getPts());
        Logcat::i("HWVC", "HwVideoInput::play audio pts=%lld", frame->getPts());
        return MEDIA_TYPE_AUDIO;
    }
    return ret;
}

bool HwVideoInput::invalidate(int tex, uint32_t width, uint32_t height) {
    Message *msg = new Message(EVENT_RENDER_FILTER, nullptr);
    msg->obj = new ObjectBox(new Size(width, height));
    msg->msg = "RENDER";
    msg->arg1 = yuvFilter->getFrameBuffer()->getFrameTexture();
    postEvent(msg);
    return true;
}

void HwVideoInput::playAudioFrame(HwAudioFrame *frame) {
    Message *msg = new Message(EVENT_SPEAKER_FEED, nullptr);
    msg->obj = frame;
    postEvent(msg);
}

void HwVideoInput::processPlayListener(int64_t us) {
    if (playListener) {
        if (llabs(lastPlayPts - us) >= INTERVAL_PROGRESS) {
            int64_t time = getCurrentTimeUS();
            playListener(us);
            Logcat::i("HWVC", "HwVideoInput::play callback cost %lld",
                      getCurrentTimeUS() - time);
            lastPlayPts = us;
        }
    }
}

void HwVideoInput::setPlayListener(function<void(int64_t)> listener) {
    this->playListener = listener;
}