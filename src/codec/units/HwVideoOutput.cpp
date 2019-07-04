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
    if (buf) {
        int ySize = 720 * 1280;
        libyuv::ABGRToI420(buf->getData(), 720 * 4,
                           videoFrame->getBuffer()->getData(), 720,
                           videoFrame->getBuffer()->getData() + ySize, 720 / 2,
                           videoFrame->getBuffer()->getData() + ySize + ySize / 4, 720 / 2,
                           720, 1280);
        videoFrame->setPts(count * 33000);
        ++count;
        if (encoder) {
            encoder->encode(videoFrame);
        } else {
            Logcat::e("HWVC", "HwVideoOutput::eventWrite failed. Encoder has release.");
        }
    } else {
        Logcat::e("HWVC", "HwVideoOutput::eventWrite failed. Buffer is null.");
    }
    return true;
}