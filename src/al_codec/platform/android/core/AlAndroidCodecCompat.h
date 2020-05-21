/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALANDROIDCODECCOMPAT_H
#define HWVC_ANDROID_ALANDROIDCODECCOMPAT_H

#include "AlCodec.h"
#include "AlMediaCodecBridge.h"

class AlAndroidCodecCompat2;

al_class AlAndroidCodecCompat al_extend AlCodec {
public:
    friend AlAndroidCodecCompat2;
public:
    AlAndroidCodecCompat(AlCodec::kID id);

    virtual ~AlAndroidCodecCompat();

    virtual HwResult configure(HwBundle &format) override;

    virtual HwResult start() override;

    /**
     * @param packet AvPacket.
     */
    virtual HwResult process(HwAbsMediaFrame **frame, HwPacket **pkt) override;

    virtual HwBuffer *getExtraBuffer(string key) override;

    virtual void flush() override;

private:
    AlAndroidCodecCompat(AlCodec::kID id, bool makeNalSelf);

    void release();

    HwResult push(uint8_t *data, size_t size, int64_t pts);

    HwResult pop(int32_t waitInUS);

private:
    static constexpr int COLOR_FormatYUV420Planar = 19;
    static constexpr int COLOR_FormatYUV420SemiPlanar = 21;
    static constexpr int COLOR_FormatYUV420Flexible = 0x7F420888;
    static constexpr int COLOR_FormatSurface = 0x7F000789;
    static constexpr int BUFFER_FLAG_SYNC_FRAME = 1;
    static constexpr int BUFFER_FLAG_KEY_FRAME = 1;
    static constexpr int BUFFER_FLAG_CODEC_CONFIG = 2;
    static constexpr int BUFFER_FLAG_END_OF_STREAM = 4;
    static constexpr int BUFFER_FLAG_PARTIAL_FRAME = 8;
    static constexpr int BUFFER_FLAG_MUXER_DATA = 16;
    static constexpr int INFO_TRY_AGAIN_LATER = -1;
    static constexpr int INFO_OUTPUT_FORMAT_CHANGED = -2;
    static constexpr int INFO_OUTPUT_BUFFERS_CHANGED = -3;
    static constexpr int CONFIGURE_FLAG_ENCODE = 1;
    AlMediaCodecBridge *bridge = nullptr;

    bool makeNalSelf = false;
    bool encodeMode = true;
    HwBuffer *buffers[4] = {nullptr, nullptr, nullptr, nullptr};
    HwBuffer *keyFrameBuf = nullptr;
    HwPacket *hwPacket = nullptr;
    HwAbsMediaFrame *outFrame = nullptr;
    int32_t fps = 25;
    int32_t stride = 0;
};


#endif //HWVC_ANDROID_ALANDROIDCODECCOMPAT_H
