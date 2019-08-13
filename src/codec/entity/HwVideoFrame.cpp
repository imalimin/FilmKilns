/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "../include/HwVideoFrame.h"
#include "Logcat.h"

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
    memcpy(destFrame->data(), data(), destFrame->size());
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
    memcpy(srcFrame->data(), data(), size());
}