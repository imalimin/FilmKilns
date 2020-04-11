/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "include/AlVideoCompiler.h"
#include "../include/HwAsyncEncoder.h"
#include "../platform/android/encoder/HwAndroidEncoder.h"
#include "libyuv.h"
#include "TimeUtils.h"
#include "../include/HwSampleFormat.h"
#include "StringUtils.h"
#include "../include/HwVideoUtils.h"

#define TAG "AlVideoCompiler"

AlVideoCompiler::AlVideoCompiler(string alias) : Unit(alias),
                                                 aFormat(HwFrameFormat::HW_SAMPLE_S32, 2, 44100) {
    registerEvent(EVENT_CANVAS_DRAW_DONE,
                  reinterpret_cast<EventFunc>(&AlVideoCompiler::_onDrawDone));
    registerEvent(MSG_TEX_READER_NOTIFY_PIXELS,
                  reinterpret_cast<EventFunc>(&AlVideoCompiler::_onWrite));

    registerEvent(EVENT_COMMON_START, reinterpret_cast<EventFunc>(&AlVideoCompiler::_onStart));
    registerEvent(EVENT_COMMON_PAUSE, reinterpret_cast<EventFunc>(&AlVideoCompiler::_onPause));
    registerEvent(EVENT_MICROPHONE_OUT_SAMPLES,
                  reinterpret_cast<EventFunc>(&AlVideoCompiler::_onSamples));
    registerEvent(MSG_VIDEO_COMPILER_BACKWARD,
                  reinterpret_cast<EventFunc>(&AlVideoCompiler::_onBackward));
    registerEvent(MSG_VIDEO_OUTPUT_PATH,
                  reinterpret_cast<EventFunc>(&AlVideoCompiler::_onSetOutPath));
    registerEvent(MSG_VIDEO_OUTPUT_SIZE,
                  reinterpret_cast<EventFunc>(&AlVideoCompiler::_onSetSize));
    registerEvent(MSG_MICROPHONE_FORMAT, reinterpret_cast<EventFunc>(&AlVideoCompiler::_onFormat));
    registerEvent(MSG_TIMESTAMP, reinterpret_cast<EventFunc>(&AlVideoCompiler::_onTimestamp));
}

AlVideoCompiler::~AlVideoCompiler() {

}

bool AlVideoCompiler::onCreate(AlMessage *msg) {
    recording = false;
    return true;
}

bool AlVideoCompiler::onDestroy(AlMessage *msg) {
    _onPause(nullptr);
    if (encoder) {
        encoder->stop();
        encoder->release();
    }
    delete encoder;
    encoder = nullptr;
    delete audioFrame;
    audioFrame = nullptr;
    delete videoFrame;
    videoFrame = nullptr;
    mPtsQueue.clear();
    return true;
}

bool AlVideoCompiler::_onDrawDone(AlMessage *msg) {
    if (recording) {
        auto *m = AlMessage::obtain(MSG_TEX_READER_REQ_PIXELS, AlMessage::QUEUE_MODE_FIRST_ALWAYS);
        m->arg1 = static_cast<int32_t>(HwFrameFormat::HW_IMAGE_NV12);
        postEvent(m);
    }
    return true;
}

bool AlVideoCompiler::_onWrite(AlMessage *msg) {
    if (!recording) {
        return true;
    }
    auto *buf = msg->getObj<AlBuffer *>();
    if (buf && !mPtsQueue.empty()) {
        int64_t pts = mPtsQueue.front();
        mPtsQueue.pop_front();
        _write(buf, pts);
    } else {
        AlLogW(TAG, "Encode failed. No pts or no buf.");
    }
    return true;
}

bool AlVideoCompiler::_onStart(AlMessage *msg) {
    _initialize();
    if (!initialized) {
        AlLogE(TAG, "failed. Not initialized.");
        return true;
    }
    recording = true;
    return true;
}

bool AlVideoCompiler::_onPause(AlMessage *msg) {
    recording = false;
    lastTsInNs = -1;
    lastATsInNs = -1;
    return true;
}

bool AlVideoCompiler::_onBackward(AlMessage *msg) {
    if (recording) {
        AlLogE(TAG, "failed. Recording now.");
        return true;
    }
    _notifyTime();
    return true;
}

void AlVideoCompiler::_initialize() {
    if (!initialized) {
        if (StringUtils::isEmpty(&path) || size.width <= 0 || size.height <= 0) {
            AlLogE(TAG, "failed");
            return;
        }
        if (0 != size.width % 16 || 0 != size.height % 16) {
            AlLogE(TAG, "Not align 16. %dx%d", size.width, size.height);
            return;
        }
        encoder = new HwAsyncEncoder();
        if (!encoder->prepare(path, size.width, size.height, aFormat)) {
            AlLogE(TAG, "Prepare video encoder failed");
        }
        videoFrame = new HwVideoFrame(nullptr, HwFrameFormat::HW_IMAGE_YV12,
                                      size.width, size.height);
        audioFrame = new HwAudioFrame(nullptr, aFormat.getFormat(), aFormat.getChannels(),
                                      aFormat.getSampleRate(), 1024);
        initialized = true;
    }
}

void AlVideoCompiler::_write(AlBuffer *buf, int64_t tsInNs) {
    if (!buf) {
        AlLogE(TAG, "failed. Buffer is null.");
        return;
    }
    //Enable NV12 or YV12
#if 1
    int pixelCount = videoFrame->getWidth() * videoFrame->getHeight();
    int64_t time = TimeUtils::getCurrentTimeUS();
    libyuv::NV12ToI420(buf->data(), videoFrame->getWidth(),
                       buf->data() + pixelCount, videoFrame->getWidth(),
                       videoFrame->data(), videoFrame->getWidth(),
                       videoFrame->data() + pixelCount,
                       videoFrame->getWidth() / 2,
                       videoFrame->data() + pixelCount * 5 / 4,
                       videoFrame->getWidth() / 2,
                       videoFrame->getWidth(), videoFrame->getHeight());
#else
    memcpy(videoFrame->data(), buf->data(), buf->size());
#endif
#if 0
    Logcat::i("HWVC", "HwVideoOutput::write nv12 convert cost %lld",
              TimeUtils::getCurrentTimeUS() - time);
#endif
    videoFrame->setPicType(HwVideoFrame::HW_PIC_DEF);
    if (lastTsInNs < 0) {
        lastTsInNs = tsInNs;
        videoFrame->setPicType(HwVideoFrame::HW_PIC_I);
    }
    vTimestamp += (tsInNs - lastTsInNs);
    lastTsInNs = tsInNs;
    videoFrame->setPts(aTimestamp / 1000);
    ++count;
    if (encoder) {
        AlLogI(TAG, "write %" PRId64, videoFrame->getPts());
        encoder->write(videoFrame);
    } else {
        AlLogE(TAG, "failed. Video encoder encoder not init.");
    }
}

bool AlVideoCompiler::_onSamples(AlMessage *msg) {
    if (!recording) {
        return true;
    }
    /// Without audio.
    if(nullptr == audioFrame) {
        return true;
    }
    auto *buf = msg->getObj<AlBuffer *>();
    int64_t tsInNs = msg->arg2;
    memcpy(audioFrame->data(), buf->data(), buf->size());
    if (lastATsInNs < 0) {
        lastATsInNs = tsInNs;
    }
    aTimestamp += (tsInNs - lastATsInNs);
    lastATsInNs = tsInNs;
    audioFrame->setPts(aTimestamp / 1000);
    if (encoder) {
        encoder->write(audioFrame);
        _notifyTime();
    } else {
        AlLogE(TAG, "failed. Audio encoder encoder not init.");
    }
    return true;
}

int64_t AlVideoCompiler::getRecordTimeInUs() {
    return max(vTimestamp / 1000, aTimestamp / 1000) + offsetOfDuration;
}

bool AlVideoCompiler::_onSetOutPath(AlMessage *msg) {
    this->path = msg->desc;
    AlLogI(TAG, "%s", this->path.c_str());
    return true;
}

bool AlVideoCompiler::_onSetSize(AlMessage *msg) {
    auto *size = msg->getObj<AlSize *>();
    if (size) {
        this->size.width = size->width;
        this->size.height = size->height;
        AlLogI(TAG, "%dx%d", size->width, size->height);
    }
    return true;
}

bool AlVideoCompiler::_onFormat(AlMessage *msg) {
    auto *f = msg->getObj<HwSampleFormat *>();
    if (f) {
        aFormat = *f;
    }
    return true;
}

bool AlVideoCompiler::_onTimestamp(AlMessage *msg) {
    mPtsQueue.push_back(msg->arg2);
    return true;
}

void AlVideoCompiler::_notifyTime() {
    // Notify record progress.
    auto *m = AlMessage::obtain(MSG_VIDEO_COMPILER_TIME);
    m->arg2 = getRecordTimeInUs();
    postMessage(m);
}
