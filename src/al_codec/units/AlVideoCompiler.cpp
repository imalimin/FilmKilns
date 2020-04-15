/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlVideoCompiler.h"
#include "AlEncoderBuilder.h"
#include "libyuv.h"
#include "TimeUtils.h"
#include "HwSampleFormat.h"
#include "StringUtils.h"
#include "HwVideoUtils.h"
#include "AlMath.h"

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
    registerEvent(MSG_VIDEO_OUTPUT_BITRATE_LEVEL,
                  reinterpret_cast<EventFunc>(&AlVideoCompiler::_onSetBitrateLevel));
    registerEvent(MSG_VIDEO_OUTPUT_PROFILE,
                  reinterpret_cast<EventFunc>(&AlVideoCompiler::_onSetProfile));
    registerEvent(MSG_VIDEO_OUTPUT_MAX_SIZE,
                  reinterpret_cast<EventFunc>(&AlVideoCompiler::_onSetMaxSize));
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
        auto *m = AlMessage::obtain(MSG_TEX_READER_REQ_PIXELS,
                                    new AlSize(this->size),
                                    AlMessage::QUEUE_MODE_FIRST_ALWAYS);
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
        int32_t width = size.width, height = size.height;
        if (width > maxSize.width && maxSize.width > 0 && maxSize.height > 0) {
            width = maxSize.width;
            height = width * size.height / size.width;
            size.width = AlMath::align16(width);
            size.height = AlMath::align16(height);
            AlLogI(TAG, "Scale size to %dx%d", size.width, size.height);
        }
        encoder = AlEncoderBuilder()
                .setOutput(path)
                .setSize(size)
                .setAudioFormat(aFormat)
                .setBitrate(size.width * size.height * bitLevel)
                .setProfile(profile)
                .setEnableAsyn(true)
                .setEnableHardware(false)
                .build();
        if (nullptr == encoder) {
            AlLogE(TAG, "Prepare video encoder failed");
            return;
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
    int64_t time = TimeUtils::getCurrentTimeUS();
    if (lastTime > 0) {
        countOfTime += (time - lastTime);
        ++count;
        if (count >= 100) {
            AlLogI(TAG, "fps %d", (count * 1000000 / countOfTime));
            countOfTime = 0;
            count = 0;
        }
    }
    lastTime = time;
    //Enable NV12 or YV12
#if 1
    int pixelCount = videoFrame->getWidth() * videoFrame->getHeight();
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
    videoFrame->setPts(vTimestamp / 1000);
    if (encoder) {
//        AlLogI(TAG, "write %"
//                PRId64, videoFrame->getPts());
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
    if (nullptr == audioFrame) {
        return true;
    }
    auto *buf = msg->getObj<AlBuffer *>();
    int64_t tsInNs = _calAudioPtsInNs(countOfSample);
    countOfSample += buf->size() / aFormat.getBytesPerSample();
    memcpy(audioFrame->data(), buf->data(), buf->size());
    if (lastATsInNs < 0) {
        lastATsInNs = tsInNs;
    }
    aTimestamp += (tsInNs - lastATsInNs);
    lastATsInNs = tsInNs;
    audioFrame->setPts(aTimestamp / 1000);
    if (encoder) {
//        AlLogI(TAG, "write %"
//                PRId64, audioFrame->getPts());
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
    if (_isInitialized()) {
        return true;
    }
    this->path = msg->desc;
    AlLogI(TAG, "%s", this->path.c_str());
    return true;
}

bool AlVideoCompiler::_onSetSize(AlMessage *msg) {
    if (_isInitialized()) {
        return true;
    }
    auto *size = msg->getObj<AlSize *>();
    if (size) {
        this->size.width = AlMath::align16(size->width);
        this->size.height = AlMath::align16(size->height);
        AlLogI(TAG, "%dx%d", this->size.width, this->size.height);
    }
    return true;
}

bool AlVideoCompiler::_onSetMaxSize(AlMessage *msg) {
    if (_isInitialized()) {
        return true;
    }
    auto *size = msg->getObj<AlSize *>();
    if (size) {
        this->maxSize.width = size->width;
        this->maxSize.height = size->height;
        AlLogI(TAG, "%dx%d", this->maxSize.width, this->maxSize.height);
    }
    return true;
}

bool AlVideoCompiler::_onFormat(AlMessage *msg) {
    if (_isInitialized()) {
        return true;
    }
    auto *f = msg->getObj<HwSampleFormat *>();
    if (f) {
        aFormat = *f;
    }
    return true;
}

bool AlVideoCompiler::_onSetBitrateLevel(AlMessage *msg) {
    if (_isInitialized()) {
        return true;
    }
    bitLevel = msg->arg1;
    if (bitLevel <= 0 || bitLevel > 5) {
        bitLevel = 3;
    }
    return true;
}

bool AlVideoCompiler::_onSetProfile(AlMessage *msg) {
    if (_isInitialized()) {
        return true;
    }
    profile = msg->desc;
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

bool AlVideoCompiler::_isInitialized() {
    return initialized;
}

int64_t AlVideoCompiler::_calAudioPtsInNs(int64_t samples) {
    return samples * 1000000000 / aFormat.getSampleRate();
}
