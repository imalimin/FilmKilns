/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwVideoOutput.h"
#include "libyuv.h"

HwVideoOutput::HwVideoOutput() : Unit() {
    name = __FUNCTION__;
    registerEvent(EVENT_COMMON_PREPARE, reinterpret_cast<EventFunc>(&HwVideoOutput::eventPrepare));
    registerEvent(EVENT_VIDEO_OUT_WRITE, reinterpret_cast<EventFunc>(&HwVideoOutput::eventWrite));
}

HwVideoOutput::~HwVideoOutput() {

}

bool HwVideoOutput::eventPrepare(Message *msg) {
    encoder = new HwFFmpegEncoder();
    if (!encoder->prepare("/sdcard/hw_encoder.mp4", 720, 1280)) {
        Logcat::e("HWVC", "HwVideoOutput::eventPrepare encoder open failed.");
    }
    videoFrame = new HwVideoFrame(nullptr, HwFrameFormat::HW_IMAGE_YV12, 720, 1280);
    return true;
}

bool HwVideoOutput::eventRelease(Message *msg) {
    if (encoder) {
        encoder->stop();
        encoder->release();
        delete encoder;
        encoder = nullptr;
    }
    if (videoFrame) {
        delete videoFrame;
        videoFrame = nullptr;
    }
    return true;
}

bool HwVideoOutput::eventWrite(Message *msg) {
    HwBuffer *buf = static_cast<HwBuffer *>(msg->obj);
    msg->obj = nullptr;
    if (!buf) {
        Logcat::e("HWVC", "HwVideoOutput::eventWrite failed. Buffer is null.");
        return true;
    }
    int pixelCount = videoFrame->getWidth() * videoFrame->getHeight();
    libyuv::ConvertToI420(buf->getData(), pixelCount,
                          videoFrame->getBuffer()->getData(), videoFrame->getWidth(),
                          videoFrame->getBuffer()->getData() + pixelCount,
                          videoFrame->getWidth() / 2,
                          videoFrame->getBuffer()->getData() + pixelCount * 5 / 4,
                          videoFrame->getWidth() / 2,
                          0, 0,
                          videoFrame->getWidth(), videoFrame->getHeight(),
                          videoFrame->getWidth(), videoFrame->getHeight(),
                          libyuv::kRotate0, libyuv::FOURCC_ABGR);
    videoFrame->setPts(count * 33000);
    ++count;
    if (encoder) {
        encoder->write(videoFrame);
    } else {
        Logcat::e("HWVC", "HwVideoOutput::eventWrite failed. Encoder has release.");
    }
    return true;
}