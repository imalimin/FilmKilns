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
    AlMediaCodecBridge *bridge = nullptr;
    bool makeNalSelf = false;
};


#endif //HWVC_ANDROID_ALANDROIDCODECCOMPAT_H
