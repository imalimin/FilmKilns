/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "../include/HwVideoFrame.h"
#include "Logcat.h"

const int32_t HwVideoFrame::HW_PIC_DEF = 0;
const int32_t HwVideoFrame::HW_PIC_I = 1;

HwVideoFrame::HwVideoFrame(HwSourcesAllocator *allocator,
                           HwFrameFormat format,
                           uint32_t width,
                           uint32_t height)
        : HwAbsMediaFrame(allocator, format, HwAbsMediaFrame::getImageSize(format, width, height)) {
    this->width = width;
    this->height = height;
}

HwVideoFrame::~HwVideoFrame() {
    setSize(0, 0);
}

void HwVideoFrame::setSize(uint32_t width, uint32_t height) {
    this->width = width;
    this->height = height;
}

uint32_t HwVideoFrame::getWidth() { return width; }

uint32_t HwVideoFrame::getHeight() { return height; }

uint64_t HwVideoFrame::duration() {
    return 0;
}

HwAbsMediaFrame *HwVideoFrame::clone() {
    HwVideoFrame *destFrame = new HwVideoFrame(nullptr, getFormat(), width, height);
    destFrame->setPts(getPts());
    destFrame->setFormat(getFormat());
    destFrame->setFlags(flags());
    if (destFrame->size() > 0) {
        memcpy(destFrame->data(), data(), destFrame->size());
    }
    return destFrame;
}

void HwVideoFrame::clone(HwAbsMediaFrame *src) {
    if (!src || !src->isVideo() || src->size() < size()) {
        Logcat::e("HWVC", "Invalid video frame");
        return;
    }
    HwVideoFrame *srcFrame = dynamic_cast<HwVideoFrame *>(src);
    srcFrame->setPts(getPts());
    srcFrame->setFormat(getFormat());
    srcFrame->setSize(getWidth(), getHeight());
    srcFrame->setPicType(getPicType());
    srcFrame->setFlags(flags());
    if (srcFrame->size() > 0 && size() > 0) {
        memcpy(srcFrame->data(), data(), size());
    }
}

void HwVideoFrame::setPicType(int32_t picType) { this->picType = picType; }

int32_t HwVideoFrame::getPicType() { return picType; }