/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwVideoOutput.h"
#include "../include/HwSequenceModel.h"
#include "../include/HwAsyncEncoder.h"
#include "../platform/android/encoder/HwAndroidEncoder.h"
#include "libyuv.h"
#include "TimeUtils.h"

HwVideoOutput::HwVideoOutput() : Unit() {
    name = __FUNCTION__;
    registerEvent(EVENT_COMMON_PREPARE, reinterpret_cast<EventFunc>(&HwVideoOutput::eventPrepare));
    registerEvent(EVENT_COMMON_PIXELS_READY,
                  reinterpret_cast<EventFunc>(&HwVideoOutput::eventResponsePixels));
    registerEvent(EVENT_COMMON_PIXELS, reinterpret_cast<EventFunc>(&HwVideoOutput::eventWrite));
    registerEvent(EVENT_COMMON_START, reinterpret_cast<EventFunc>(&HwVideoOutput::eventStart));
    registerEvent(EVENT_COMMON_PAUSE, reinterpret_cast<EventFunc>(&HwVideoOutput::eventPause));
    registerEvent(EVENT_MICROPHONE_OUT_SAMPLES,
                  reinterpret_cast<EventFunc>(&HwVideoOutput::eventSamples));
}

HwVideoOutput::~HwVideoOutput() {

}

int HwVideoOutput::getWidth() {
    return static_cast<HwSequenceModel *>(getModel())->getCodecConfig()->width;
}

int HwVideoOutput::getHeight() {
    return static_cast<HwSequenceModel *>(getModel())->getCodecConfig()->height;
}

bool HwVideoOutput::eventPrepare(Message *msg) {
    recording = false;
    encoder = new HwAsyncEncoder();
    if (!encoder->prepare(static_cast<HwSequenceModel *>(getModel())->getCodecConfig()->path,
                          getWidth(), getHeight())) {
        Logcat::e("HWVC", "HwVideoOutput::eventPrepare encoder open failed.");
    }
    videoFrame = new HwVideoFrame(nullptr, HwFrameFormat::HW_IMAGE_YV12, getWidth(), getHeight());
    audioFrame = new HwAudioFrame(nullptr, HwFrameFormat::HW_SAMPLE_S32, 2, 44100, 1024);
    return true;
}

bool HwVideoOutput::eventRelease(Message *msg) {
    if (encoder) {
        encoder->stop();
        encoder->release();
        delete encoder;
        encoder = nullptr;
    }
    if (audioFrame) {
        delete audioFrame;
        audioFrame = nullptr;
    }
    if (videoFrame) {
        delete videoFrame;
        videoFrame = nullptr;
    }
    return true;
}

bool HwVideoOutput::eventResponsePixels(Message *msg) {
    if (recording) {
        postEvent(new Message(EVENT_COMMON_PIXELS_READ, nullptr,
                              Message::QUEUE_MODE_FIRST_ALWAYS, nullptr));
    }
    return true;
}

bool HwVideoOutput::eventWrite(Message *msg) {
    if (!recording) {
        return true;
    }
    HwBuffer *buf = static_cast<HwBuffer *>(msg->obj);
    write(buf, msg->arg2);
    return true;
}

bool HwVideoOutput::eventStart(Message *msg) {
    recording = true;
    return true;
}

bool HwVideoOutput::eventPause(Message *msg) {
    recording = false;
    lastTsInNs = -1;
    lastATsInNs = -1;
    return true;
}

void HwVideoOutput::write(HwBuffer *buf, int64_t tsInNs) {
    if (!buf) {
        Logcat::e("HWVC", "HwVideoOutput::write failed. Buffer is null.");
        return;
    }
    //Enable NV12 or YV12
#if 0
    int pixelCount = videoFrame->getWidth() * videoFrame->getHeight();
    int64_t time = TimeUtils::getCurrentTimeUS();
    libyuv::NV12ToI420(buf->getData(), videoFrame->getWidth(),
                       buf->getData() + pixelCount, videoFrame->getWidth(),
                       videoFrame->getBuffer()->getData(), videoFrame->getWidth(),
                       videoFrame->getBuffer()->getData() + pixelCount,
                       videoFrame->getWidth() / 2,
                       videoFrame->getBuffer()->getData() + pixelCount * 5 / 4,
                       videoFrame->getWidth() / 2,
                       videoFrame->getWidth(), videoFrame->getHeight());
#else
    memcpy(videoFrame->getBuffer()->getData(), buf->getData(), buf->size());
#endif
#if 0
    Logcat::i("HWVC", "HwVideoOutput::write nv12 convert cost %lld",
              TimeUtils::getCurrentTimeUS() - time);
#endif
    if (lastTsInNs < 0) {
        lastTsInNs = tsInNs;
    }
    timestamp += (tsInNs - lastTsInNs);
    lastTsInNs = tsInNs;
    videoFrame->setPts(timestamp / 1000);
    ++count;
    if (encoder) {
        encoder->write(videoFrame);
    } else {
        Logcat::e("HWVC", "HwVideoOutput::write failed. Encoder has release.");
    }
}

bool HwVideoOutput::eventSamples(Message *msg) {
    if (!recording) {
        return true;
    }
    HwBuffer *buf = static_cast<HwBuffer *>(msg->obj);
    int64_t tsInNs = msg->arg2;
    memcpy(audioFrame->getBuffer()->getData(), buf->getData(), buf->size());
    if (lastATsInNs < 0) {
        lastATsInNs = tsInNs;
    }
    aTimestamp += (tsInNs - lastATsInNs);
    lastATsInNs = tsInNs;
    audioFrame->setPts(aTimestamp / 1000);
    if (encoder) {
        encoder->write(audioFrame);
    } else {
        Logcat::e("HWVC", "HwVideoOutput::write audio failed. Encoder has release.");
    }
    return true;
}