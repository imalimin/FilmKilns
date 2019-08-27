/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwVideoCompiler.h"
#include "../include/HwAsyncEncoder.h"
#include "../platform/android/encoder/HwAndroidEncoder.h"
#include "libyuv.h"
#include "TimeUtils.h"
#include "../include/HwSampleFormat.h"
#include "StringUtils.h"

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
    remuxer();
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
    track.backward();
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
    memcpy(videoFrame->data(), buf->getData(), buf->size());
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
    timestamp += (tsInNs - lastTsInNs);
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
            clip.start = audioFrame->getPts();
        }
        clip.end = audioFrame->getPts();
        encoder->write(audioFrame);
    } else {
        Logcat::e("HWVC", "HwVideoCompiler::write audio failed. Encoder has release.");
    }
    return true;
}

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
    clips.push_front(clip);
}

HwVideoCompiler::HwClip &HwVideoCompiler::HwTrack::backward() {
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

void HwVideoCompiler::remuxer() {
    string path = getString("path");
    av_register_all();
    /**
     * Open input
     */
    AVFormatContext *ctx = avformat_alloc_context();
    if (avformat_open_input(&ctx, path.c_str(), NULL, NULL) != 0) {
        LOGE("Couldn't open input stream.");
        return;
    }
    if (avformat_find_stream_info(ctx, NULL) < 0) {
        LOGE("Couldn't find stream information.");
        return;
    }
    int32_t videoTrack = -1, audioTrack = -1;
    for (int i = 0; i < ctx->nb_streams; i++) {
        if (-1 == videoTrack &&
            AVMediaType::AVMEDIA_TYPE_VIDEO == ctx->streams[i]->codecpar->codec_type) {
            videoTrack = i;
        }
        if (-1 == audioTrack &&
            AVMediaType::AVMEDIA_TYPE_AUDIO == ctx->streams[i]->codecpar->codec_type) {
            audioTrack = i;
        }
    }
    /**
     * Open output
     */
    string opath = string(path);
    int32_t ovideoTrack = -1, oaudioTrack = -1;
    opath.append(".remuxer.mp4");
    AVFormatContext *octx = nullptr;
    int ret = avformat_alloc_output_context2(&octx, NULL, "MP4", opath.c_str());
    if (ret < 0 || !octx) {
        Logcat::e("HWVC", "HwFFMuxer::configure failed %s", strerror(AVUNERROR(ret)));
        return;
    }
    av_dict_set(&octx->metadata, "comment", "hwvc", 0);
    if (videoTrack >= 0) {
        AVStream *stream = avformat_new_stream(octx, nullptr);
        avcodec_parameters_copy(stream->codecpar, ctx->streams[videoTrack]->codecpar);
        ovideoTrack = stream->index;
    }
    if (audioTrack >= 0) {
        AVStream *stream = avformat_new_stream(octx, nullptr);
        avcodec_parameters_copy(stream->codecpar, ctx->streams[audioTrack]->codecpar);
        oaudioTrack = stream->index;
    }
    if (avio_open2(&octx->pb, opath.c_str(), AVIO_FLAG_WRITE, nullptr, nullptr) < 0) {
        return;
    }
    ret = avformat_write_header(octx, nullptr);
    /*
     * Start remxuer.
     */
    AVPacket *avPacket = av_packet_alloc();
    ret = 0;
    bool reset = false;
    int64_t aTime = 0, vTime = 0;
    int64_t aDTime = 0, vDTime = 0;
    int64_t aLastTime = 0, vLastTime = 0;
    int64_t aDLastTime = 0, vDLastTime = 0;
    while (AVERROR_EOF != ret) {
        ret = av_read_frame(ctx, avPacket);
        if (0 == ret) {
            int64_t pts = av_rescale_q_rnd(avPacket->pts,
                                           ctx->streams[avPacket->stream_index]->time_base,
                                           AV_TIME_BASE_Q,
                                           AV_ROUND_NEAR_INF);
            if (!track.contain(pts)) {
                if (videoTrack == avPacket->stream_index) {
                    vLastTime = avPacket->pts;
                    vDLastTime = avPacket->dts;
                } else if (audioTrack == avPacket->stream_index) {
                    aLastTime = avPacket->pts;
                    aDLastTime = avPacket->dts;
                }
                av_packet_unref(avPacket);
                continue;
            }
            if (videoTrack == avPacket->stream_index) {
//                Logcat::i("hwvc", "HwFFmpegEncoder::write %d", avPacket->flags & AV_PKT_FLAG_KEY);
                vTime += (avPacket->pts - vLastTime);
                vDTime += (avPacket->dts - vDLastTime);
                vLastTime = avPacket->pts;
                vDLastTime = avPacket->dts;
                avPacket->pts = vTime;
                avPacket->dts = vDTime;
                avPacket->stream_index = ovideoTrack;
            } else if (audioTrack == avPacket->stream_index) {
                aTime += (avPacket->pts - aLastTime);
                aDTime += (avPacket->dts - aDLastTime);
                aLastTime = avPacket->pts;
                aDLastTime = avPacket->dts;
                avPacket->pts = aTime;
                avPacket->dts = aDTime;
                avPacket->stream_index = oaudioTrack;
            } else {
                av_packet_unref(avPacket);
                continue;
            }
            int flag = av_interleaved_write_frame(octx, avPacket);
            av_packet_unref(avPacket);
            Logcat::i("hwvc", "remuxer end. write %lld, ret = %d", pts, flag);
        }
    }
    /*
     * Release output
     */
    if (octx) {
        av_write_trailer(octx);
        if (!(octx->flags & AVFMT_NOFILE)) {
            avio_closep(&octx->pb);
        }
        avformat_free_context(octx);
        octx = nullptr;
    }
    Logcat::i("hwvc", "remuxer end.");
    if (ctx) {
        if (!(ctx->flags & AVFMT_NOFILE)) {
            avio_closep(&ctx->pb);
        }
        avformat_free_context(ctx);
        ctx = nullptr;
    }
    if (avPacket) {
        av_packet_free(&avPacket);
        avPacket = nullptr;
    }
}