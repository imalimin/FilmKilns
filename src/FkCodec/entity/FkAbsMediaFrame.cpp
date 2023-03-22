/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "FkAbsMediaFrame.h"

FK_IMPL_CLASS_TYPE(FkAbsMediaFrame, FkAbsFrame)

AVSampleFormat FkAbsMediaFrame::convertAudioFrameFormat(kFrameFormat format) {
    if (format >= kFrameFormat::SAMPLE_U8 && format < kFrameFormat::SAMPLE_END) {
        return static_cast<AVSampleFormat>(static_cast<int>(format) -
                                           static_cast<int>(kFrameFormat::SAMPLE_U8));
    }
    return AV_SAMPLE_FMT_NONE;
}

kFrameFormat FkAbsMediaFrame::convertToAudioFrameFormat(AVSampleFormat format) {
    if (format >= AV_SAMPLE_FMT_U8 && format < AV_SAMPLE_FMT_NB) {
        return static_cast<kFrameFormat>(static_cast<int>(format) +
                                          static_cast<int>(kFrameFormat::SAMPLE_U8));
    }
    return kFrameFormat::NONE;
}

int FkAbsMediaFrame::getBytesPerSample(kFrameFormat format) {
    return av_get_bytes_per_sample(convertAudioFrameFormat(format));
}

AVPixelFormat FkAbsMediaFrame::convertVideoFrameFormat(kFrameFormat format) {
    switch (format) {
        case kFrameFormat::IMAGE_YV12:
            return AV_PIX_FMT_YUV420P;
        case kFrameFormat::IMAGE_NV12:
            return AV_PIX_FMT_NV12;
        case kFrameFormat::IMAGE_RGB:
            return AV_PIX_FMT_RGB24;
        case kFrameFormat::IMAGE_RGBA:
            return AV_PIX_FMT_RGBA;
        default:
            return AV_PIX_FMT_NONE;
    }
}

kFrameFormat FkAbsMediaFrame::convertToVideoFrameFormat(AVPixelFormat format) {
    switch (format) {
        case AV_PIX_FMT_YUV420P:
            return kFrameFormat::IMAGE_YV12;
        case AV_PIX_FMT_NV12:
            return kFrameFormat::IMAGE_NV12;
        case AV_PIX_FMT_BGRA:
        case AV_PIX_FMT_RGBA:
            return kFrameFormat::IMAGE_RGBA;
        default:
            return kFrameFormat::NONE;
    }
}

int FkAbsMediaFrame::getImageSize(kFrameFormat format, int width, int height) {
    switch (format) {
        case kFrameFormat::IMAGE_RGB:
            return width * height * 3;
        case kFrameFormat::IMAGE_RGBA:
            return width * height * 4;
        case kFrameFormat::IMAGE_YV12:
        case kFrameFormat::IMAGE_NV12:
            return width * height * 3 / 2;
        default:
            return 0;
    }
}

FkAbsMediaFrame::FkAbsMediaFrame(FkSourcesAllocator *allocator,
                                 kFrameFormat format,
                                 size_t size) : FkSources(allocator),
                                                FkAbsFrame(size) {
    this->format = format;
}

FkAbsMediaFrame::~FkAbsMediaFrame() {
}

void FkAbsMediaFrame::setFormat(kFrameFormat _format) { this->format = _format; }

kFrameFormat FkAbsMediaFrame::getFormat() { return format; }

void FkAbsMediaFrame::setPts(int64_t _pts) { this->pts = _pts; }

int64_t FkAbsMediaFrame::getPts() { return pts; }

bool FkAbsMediaFrame::isVideo() {
    return getFormat() >= kFrameFormat::IMAGE_RGB && getFormat() < kFrameFormat::IMAGE_END;
}

bool FkAbsMediaFrame::isAudio() {
    return getFormat() >= kFrameFormat::SAMPLE_U8 && getFormat() < kFrameFormat::SAMPLE_END;
}

int32_t FkAbsMediaFrame::flags() {
    return _flags;
}

void FkAbsMediaFrame::setFlags(int32_t flags) {
    _flags = flags;
}
