/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwVideoCompiler.h"
#include "../include/HwAsyncEncoder.h"
#include "../platform/android/encoder/HwAndroidEncoder.h"
#include "libyuv.h"
#include "TimeUtils.h"
#include "../include/HwSampleFormat.h"
#include "StringUtils.h"
#include "../include/HwVideoUtils.h"

HwVideoCompiler::HwVideoCompiler(string alias) : Unit(alias) {
    registerEvent(EVENT_COMMON_PREPARE,
                  reinterpret_cast<EventFunc>(&HwVideoCompiler::eventPrepare));
    registerEvent(EVENT_COMMON_PIXELS_READY,
                  reinterpret_cast<EventFunc>(&HwVideoCompiler::eventResponsePixels));
    registerEvent(EVENT_COMMON_PIXELS, reinterpret_cast<EventFunc>(&HwVideoCompiler::eventWrite));
    registerEvent(EVENT_COMMON_START, reinterpret_cast<EventFunc>(&HwVideoCompiler::eventStart));
    registerEvent(EVENT_COMMON_PAUSE, reinterpret_cast<EventFunc>(&HwVideoCompiler::eventPause));
    registerEvent(EVENT_MICROPHONE_OUT_SAMPLES,
                  reinterpret_cast<EventFunc>(&HwVideoCompiler::eventSamples));
    registerEvent(EVENT_VIDEO_COMPILER_BACKWARD,
                  reinterpret_cast<EventFunc>(&HwVideoCompiler::eventBackward));
}

HwVideoCompiler::~HwVideoCompiler() {

}

int HwVideoCompiler::getWidth() {
    return getInt32("width");
}

int HwVideoCompiler::getHeight() {
    return getInt32("height");
}

bool HwVideoCompiler::eventPrepare(Message *msg) {
    recording = false;
    int32_t width = getInt32("width");
    int32_t height = getInt32("height");
    string path = getString("path");
    HwSampleFormat *format = dynamic_cast<HwSampleFormat *>(getObject("audioFormat"));
    if (StringUtils::isEmpty(&path) || !format || HwBundle::VALUE_NONE == width ||
        HwBundle::VALUE_NONE == height) {
        Logcat::e("HWVC", "HwVideoCompiler::eventPrepare failed.Invalid arguments.");
        return true;
    }
    encoder = new HwAsyncEncoder();
    if (!encoder->prepare(path, width, height, *format)) {
        Logcat::e("HWVC", "HwVideoCompiler::eventPrepare encoder open failed.");
    }
    videoFrame = new HwVideoFrame(nullptr, HwFrameFormat::HW_IMAGE_YV12, width, height);
    audioFrame = new HwAudioFrame(nullptr, format->getFormat(), format->getChannels(),
                                  format->getSampleRate(), 1024);
    return true;
}

bool HwVideoCompiler::eventRelease(Message *msg) {
    eventPause(nullptr);
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
    this->recordListener = nullptr;
    if (requestReMux) {
        remux();
    }
    return true;
}

bool HwVideoCompiler::eventResponsePixels(Message *msg) {
    if (recording) {
        postEvent(new Message(EVENT_COMMON_PIXELS_READ, nullptr,
                              Message::QUEUE_MODE_FIRST_ALWAYS, nullptr));
    }
    return true;
}

bool HwVideoCompiler::eventWrite(Message *msg) {
    if (!recording) {
        return true;
    }
    HwBuffer *buf = static_cast<HwBuffer *>(msg->obj);
    write(buf, msg->arg2);
    return true;
}

bool HwVideoCompiler::eventStart(Message *msg) {
    recording = true;
    return true;
}

bool HwVideoCompiler::eventPause(Message *msg) {
    recording = false;
    if (!clip.empty()) {
        track.put(clip.start, clip.end);
        clip.clear();
    }
    lastTsInNs = -1;
    lastATsInNs = -1;
    return true;
}

bool HwVideoCompiler::eventBackward(Message *msg) {
    if (recording) {
        Logcat::e("HWVC", "HwVideoCompiler::eventBackward failed. Recording now.");
        return true;
    }
    auto clip = track.backward();
    offsetOfDuration -= clip.duration();
    requestReMux = true;
    // Notify record progress.
    if (recordListener) {
        recordListener(getRecordTimeInUs());
    }
    return true;
}

void HwVideoCompiler::write(HwBuffer *buf, int64_t tsInNs) {
    if (!buf) {
        Logcat::e("HWVC", "HwVideoCompiler::write failed. Buffer is null.");
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
        encoder->write(videoFrame);
    } else {
        Logcat::e("HWVC", "HwVideoCompiler::write failed. Encoder has release.");
    }
}

bool HwVideoCompiler::eventSamples(Message *msg) {
    if (!recording) {
        return true;
    }
    HwBuffer *buf = static_cast<HwBuffer *>(msg->obj);
    int64_t tsInNs = msg->arg2;
    memcpy(audioFrame->data(), buf->data(), buf->size());
    if (lastATsInNs < 0) {
        lastATsInNs = tsInNs;
    }
    aTimestamp += (tsInNs - lastATsInNs);
    lastATsInNs = tsInNs;
    audioFrame->setPts(aTimestamp / 1000);
    if (encoder) {
        if (clip.empty()) {
            clip.start = track.lastTrimOut();
        }
        clip.end = videoFrame->getPts();
        encoder->write(audioFrame);
        // Notify record progress.
        if (recordListener) {
            recordListener(getRecordTimeInUs());
        }
    } else {
        Logcat::e("HWVC", "HwVideoCompiler::write audio failed. Encoder has release.");
    }
    return true;
}

void HwVideoCompiler::setRecordListener(function<void(int64_t)> listener) {
    this->recordListener = listener;
}

int64_t HwVideoCompiler::getRecordTimeInUs() {
    return max(vTimestamp / 1000, aTimestamp / 1000) + offsetOfDuration;
}

//********************************
// HwVideoCompiler end
//********************************

HwVideoCompiler::HwClip::HwClip() : HwClip(-1, -1) {

}

HwVideoCompiler::HwClip::HwClip(int64_t start, int64_t end) : Object(), start(start), end(end) {

}

HwVideoCompiler::HwClip::HwClip(const HwVideoCompiler::HwClip &value) : Object(),
                                                                        start(value.start),
                                                                        end(value.end) {

}

HwVideoCompiler::HwClip::~HwClip() {
    start = 0;
    end = 0;
}

HwVideoCompiler::HwClip &HwVideoCompiler::HwClip::operator=(const HwVideoCompiler::HwClip &value) {
    this->start = value.start;
    this->end = value.end;
    return *this;
}

int64_t HwVideoCompiler::HwClip::duration() {
    if (start > end) return 0;
    return end - start;
}

bool HwVideoCompiler::HwClip::empty() {
    return start < 0 && end < 0;
}

void HwVideoCompiler::HwClip::clear() {
    this->start = -1;
    this->end = -1;
}

HwVideoCompiler::HwTrack::HwTrack() : Object() {

}

HwVideoCompiler::HwTrack::~HwTrack() {
    std::lock_guard<std::mutex> guard(mtx);
    clips.clear();
}

void HwVideoCompiler::HwTrack::put(int64_t start, int64_t end) {
    std::lock_guard<std::mutex> guard(mtx);
    HwClip clip = HwClip(start, end);
    durationInUs += clip.duration();
    lastTrimOutInUs = clip.end;
    clips.push_front(clip);
}

HwVideoCompiler::HwClip HwVideoCompiler::HwTrack::backward() {
    std::lock_guard<std::mutex> guard(mtx);
    if (clips.empty()) {
        HwClip clip = HwClip(-1, -1);
        return clip;
    }
    HwClip clip = clips.front();
    durationInUs -= clip.duration();
    clips.pop_front();
    return clip;
}

int64_t HwVideoCompiler::HwTrack::duration() {
    std::lock_guard<std::mutex> guard(mtx);
    return durationInUs;
}
int64_t HwVideoCompiler::HwTrack::lastTrimOut() {
    std::lock_guard<std::mutex> guard(mtx);
    return lastTrimOutInUs;
}

bool HwVideoCompiler::HwTrack::contain(int64_t pts) {
    auto itr = clips.begin();
    while (clips.end() != itr) {
        if (pts >= (*itr).start && pts < (*itr).end) {
            return true;
        }
        ++itr;
    }
    return false;
}

void HwVideoCompiler::HwTrack::get(std::vector<int64_t> *trimIns,
                                   std::vector<int64_t> *trimOuts) {
    auto itr = clips.begin();
    while (itr != clips.end()) {
        trimIns->push_back(itr->start);
        trimOuts->push_back(itr->end);
        ++itr;
    }
}

void HwVideoCompiler::remux() {
    std::vector<int64_t> trimIns;
    std::vector<int64_t> trimOuts;
    track.get(&trimIns, &trimOuts);
    string input = getString("path");
    string output = string(input);
    output.append(".remux.mp4");
    if (trimIns.size() > 1 && trimOuts.size() > 1 && trimIns.size() == trimOuts.size()) {
        HwResult ret = HwVideoUtils::remuxCopy(input, output, trimIns, trimOuts);
        std::string path = std::string(input);
        path.append(".src.mp4");
        int r = rename(input.c_str(), path.c_str());
        r = rename(output.c_str(), input.c_str());
    }
}