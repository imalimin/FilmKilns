/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALANDROIDCODECCOMPAT_H
#define HWVC_ANDROID_ALANDROIDCODECCOMPAT_H

#include "HwAbsCodec.h"
#include "AlMediaCodecBridge.h"

al_class_ex(AlAndroidCodecCompat, HwAbsCodec) {
public:
    AlAndroidCodecCompat(int32_t codecId, bool makeNalSelf = false);

    virtual ~AlAndroidCodecCompat();

    virtual HwResult configure(HwBundle &format) override;

    virtual HwResult start() override;

    /**
     * @param packet AvPacket.
     */
    virtual HwResult process(HwAbsMediaFrame **frame, HwPacket **pkt) override;

    virtual int32_t type() override;

    virtual HwBuffer *getExtraBuffer(string key) override;

    virtual void flush() override;

private:
    static constexpr int COLOR_FormatYUV420Flexible = 0x7F420888;
    AlMediaCodecBridge *bridge = nullptr;
    bool makeNalSelf = false;
    int32_t fps = 25;
    HwPacket *packet = nullptr;
};


#endif //HWVC_ANDROID_ALANDROIDCODECCOMPAT_H
