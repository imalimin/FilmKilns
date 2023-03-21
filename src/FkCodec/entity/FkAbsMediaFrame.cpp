/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "FkAbsMediaFrame.h"

AVSampleFormat HwAbsMediaFrame::convertAudioFrameFormat(HwFrameFormat format) {
    if (format >= HwFrameFormat::HW_SAMPLE_U8 && format < HwFrameFormat::HW_SAMPLE_END) {
        return static_cast<AVSampleFormat>(static_cast<int>(format) -
                                           static_cast<int>(HwFrameFormat::HW_SAMPLE_U8));
    }
    return AV_SAMPLE_FMT_NONE;
}

HwFrameFormat HwAbsMediaFrame::convertToAudioFrameFormat(AVSampleFormat format) {
    if (format >= AV_SAMPLE_FMT_U8 && format < AV_SAMPLE_FMT_NB) {
        return static_cast<HwFrameFormat>(static_cast<int>(format) +
                                          static_cast<int>(HwFrameFormat::HW_SAMPLE_U8));
    }
    return HwFrameFormat::HW_FMT_NONE;
}

int HwAbsMediaFrame::getBytesPerSample(HwFrameFormat format) {
    return av_get_bytes_per_sample(convertAudioFrameFormat(format));
}

AVPixelFormat HwAbsMediaFrame::convertVideoFrameFormat(HwFrameFormat format) {
    switch (format) {
        case HwFrameFormat::HW_IMAGE_YV12:
            return AV_PIX_FMT_YUV420P;
        case HwFrameFormat::HW_IMAGE_NV12:
            return AV_PIX_FMT_NV12;
        case HwFrameFormat::HW_IMAGE_RGB:
            return AV_PIX_FMT_RGB24;
        case HwFrameFormat::HW_IMAGE_RGBA:
            return AV_PIX_FMT_RGBA;
        default:
            return AV_PIX_FMT_NONE;
    }
}

HwFrameFormat HwAbsMediaFrame::convertToVideoFrameFormat(AVPixelFormat format) {
    switch (format) {
        case AV_PIX_FMT_YUV420P:
            return HwFrameFormat::HW_IMAGE_YV12;
        case AV_PIX_FMT_NV12:
            return HwFrameFormat::HW_IMAGE_NV12;
        case AV_PIX_FMT_BGRA:
        case AV_PIX_FMT_RGBA:
            return HwFrameFormat::HW_IMAGE_RGBA;
        default:
            return HwFrameFormat::HW_FMT_NONE;
    }
}

int HwAbsMediaFrame::getImageSize(HwFrameFormat format, int width, int height) {
    switch (format) {
        case HwFrameFormat::HW_IMAGE_RGB:
            return width * height * 3;
        case HwFrameFormat::HW_IMAGE_RGBA:
            return width * height * 4;
        case HwFrameFormat::HW_IMAGE_YV12:
        case HwFrameFormat::HW_IMAGE_NV12:
            return width * height * 3 / 2;
        default:
            return 0;
    }
}

HwAbsMediaFrame::HwAbsMediaFrame(HwSourcesAllocator *allocator,
                                 HwFrameFormat format,
                                 size_t size) : HwSources(allocator),
                                                HwAbsFrame(size) {
    this->format = format;
}

HwAbsMediaFrame::~HwAbsMediaFrame() {
}

void HwAbsMediaFrame::setFormat(HwFrameFormat format) { this->format = format; }

HwFrameFormat HwAbsMediaFrame::getFormat() { return format; }

void HwAbsMediaFrame::setPts(int64_t pts) { this->pts = pts; }

int64_t HwAbsMediaFrame::getPts() { return pts; }

bool HwAbsMediaFrame::isVideo() {
    return getFormat() >= HwFrameFormat::HW_IMAGE_RGB && getFormat() < HwFrameFormat::HW_IMAGE_END;
}

bool HwAbsMediaFrame::isAudio() {
    return getFormat() >= HwFrameFormat::HW_SAMPLE_U8 && getFormat() < HwFrameFormat::HW_SAMPLE_END;
}

int32_t HwAbsMediaFrame::flags() {
    return _flags;
}

void HwAbsMediaFrame::setFlags(int32_t flags) {
    _flags = flags;
}
