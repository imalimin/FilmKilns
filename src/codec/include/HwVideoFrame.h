/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HARDWAREVIDEOCODEC_HWVIDEOFRAME_H
#define HARDWAREVIDEOCODEC_HWVIDEOFRAME_H

#include "HwAbsMediaFrame.h"

class HwVideoFrame : public HwAbsMediaFrame {
public:
    static const int32_t HW_PIC_DEF;
    /** I frame */
    static const int32_t HW_PIC_I;
public:
    HwVideoFrame(HwSourcesAllocator *allocator,
                 HwFrameFormat format,
                 uint32_t width,
                 uint32_t height);

    virtual ~HwVideoFrame();

    void setSize(uint32_t width, uint32_t height);

    uint32_t getWidth();

    uint32_t getHeight();

    virtual uint64_t duration();

    virtual HwAbsMediaFrame *clone();

    virtual void clone(HwAbsMediaFrame *src);

    void setPicType(int32_t picType);

    int32_t getPicType();

private:
    uint32_t width = 0;
    uint32_t height = 0;
    int32_t picType = HW_PIC_DEF;
};


#endif //HARDWAREVIDEOCODEC_HWVIDEOFRAME_H
