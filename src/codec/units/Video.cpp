/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/
#include "../include/Video.h"
#include "ObjectBox.h"
#include "Size.h"
#include "TimeUtils.h"
#include "../include/HwVideoFrame.h"
#include "../include/HwAudioFrame.h"

Video::Video() : HwStreamMedia() {
    name = __FUNCTION__;
    registerEvent(EVENT_COMMON_PREPARE, reinterpret_cast<EventFunc>(&Video::eventPrepare));
    registerEvent(EVENT_VIDEO_START, reinterpret_cast<EventFunc>(&Video::eventStart));
    registerEvent(EVENT_VIDEO_PAUSE, reinterpret_cast<EventFunc>(&Video::eventPause));
    registerEvent(EVENT_VIDEO_SEEK, reinterpret_cast<EventFunc>(&Video::eventSeek));
    registerEvent(EVENT_VIDEO_SET_SOURCE, reinterpret_cast<EventFunc>(&Video::eventSetSource));
    registerEvent(EVENT_VIDEO_LOOP, reinterpret_cast<EventFunc>(&Video::eventLoop));
    decoder = new AsynVideoDecoder();
}

Video::Video(HandlerThread *handlerThread) : HwStreamMedia(handlerThread) {
    name = __FUNCTION__;
    registerEvent(EVENT_COMMON_PREPARE, reinterpret_cast<EventFunc>(&Video::eventPrepare));
    registerEvent(EVENT_VIDEO_START, reinterpret_cast<EventFunc>(&Video::eventStart));
    registerEvent(EVENT_VIDEO_PAUSE, reinterpret_cast<EventFunc>(&Video::eventPause));
    registerEvent(EVENT_VIDEO_SEEK, reinterpret_cast<EventFunc>(&Video::eventSeek));
    registerEvent(EVENT_VIDEO_SET_SOURCE, reinterpret_cast<EventFunc>(&Video::eventSetSource));
    registerEvent(EVENT_VIDEO_LOOP, reinterpret_cast<EventFunc>(&Video::eventLoop));
    decoder = new AsynVideoDecoder();
}

Video::~Video() {
    LOGI("Video::~Video");
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
}

bool Video::eventRelease(Message *msg) {
    LOGI("Video::eventRelease");
    eventStop(nullptr);
    if (texAllocator) {
        delete texAllocator;
        texAllocator = nullptr;
    }
    return true;
}

bool Video::eventPrepare(Message *msg) {
    playState = PAUSE;
    if (!decoder->prepare(path)) {
        LOGE("Video::open %s failed", path);
        return true;
    }
    if (!texAllocator) {
        texAllocator = new TextureAllocator();
    }
    return true;
}

bool Video::eventStart(Message *msg) {
    LOGI("Video::eventStart");
    if (PAUSE == playState) {
        playState = PLAYING;
        if (decoder) {
            decoder->start();
        }
        loop();
    }
    return true;
}

bool Video::eventPause(Message *msg) {
    if (STOP != playState) {
        playState = PAUSE;
    }
    if (decoder) {
        decoder->pause();
    }
    return true;
}

bool Video::eventSeek(Message *msg) {
    int64_t us = msg->arg2;
    decoder->seek(us);
    return true;
}

bool Video::eventStop(Message *msg) {
    playState = STOP;
    return true;
}

bool Video::eventSetSource(Message *msg) {
    this->path = static_cast<char *>(msg->tyrUnBox());
    return true;
}

void Video::loop() {
    postEvent(new Message(EVENT_VIDEO_LOOP, nullptr));
}

bool Video::eventLoop(Message *msg) {
    if (PLAYING != playState) {
        return false;
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

void Video::checkFilter() {
    if (!yuvFilter) {
        yuvFilter = new YUV420PFilter();
        yuvFilter->init(decoder->width(), decoder->height());
        yuv[0] = texAllocator->alloc();
        yuv[1] = texAllocator->alloc();
        yuv[2] = texAllocator->alloc();
    }
}

int Video::grab() {
    int64_t time = getCurrentTimeUS();
    HwAbsMediaFrame *frame = nullptr;
    int ret = decoder->grab(&frame);
    Logcat::i("HWVC", "Video::grab cost: %lld, ret: %d", getCurrentTimeUS() - time, ret);
    if (!frame) {
        return ret;
    }

    if (frame->isVideo()) {
        Logcat::i("HWVC", "Video::play picture");
        HwVideoFrame *videoFrame = dynamic_cast<HwVideoFrame *>(frame);
        lastShowTime = getCurrentTimeUS();
        int64_t curPts = frame->getPts();
        if (lastPts > 0) {
            int64_t t = (curPts - lastPts) - (getCurrentTimeUS() - lastShowTime);
            simpleLock.wait(t);
            LOGI("Video::grab %d x %d, delta time: %lld, wait time: %lld",
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
        Logcat::i("HWVC", "Video::play audio: %d, %lld, %lld",
                  audioFrame->getChannels(),
                  audioFrame->getSampleCount(),
                  audioFrame->getBufferSize());
        return MEDIA_TYPE_AUDIO;
    }
    return ret;
}

bool Video::invalidate(int tex, uint32_t width, uint32_t height) {
    Message *msg = new Message(EVENT_RENDER_FILTER, nullptr);
    msg->obj = new ObjectBox(new Size(width, height));
    msg->arg1 = yuvFilter->getFrameBuffer()->getFrameTexture();
    postEvent(msg);
    return true;
}