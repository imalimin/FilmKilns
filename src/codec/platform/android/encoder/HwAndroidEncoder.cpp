/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "HwAndroidEncoder.h"
#include "Logcat.h"
#include "../core/HwAndroidCodec.h"
#include "../../../include/HwFFMuxer.h"

HwAndroidEncoder::HwAndroidEncoder() : HwAbsVideoEncoder() {

}

HwAndroidEncoder::~HwAndroidEncoder() {
    release();
}

bool HwAndroidEncoder::prepare(string path, int width, int height, HwSampleFormat audioFormat) {
    this->path = path;
    this->width = width;
    this->height = height;
    this->audioFormat = audioFormat;
    return configure();
}

bool HwAndroidEncoder::configure() {
    vCodec = new HwAndroidCodec(HwAndroidCodec::HW_ANDROID_AVC);
    HwBundle format;
    format.putInt32(HwAbsCodec::KEY_FORMAT, static_cast<int32_t>(HwFrameFormat::HW_IMAGE_YV12));
    format.putInt32(HwAbsCodec::KEY_WIDTH, width);
    format.putInt32(HwAbsCodec::KEY_HEIGHT, height);
    format.putInt32(HwAbsCodec::KEY_BIT_RATE, width * height * 3);
    format.putInt32(HwAbsCodec::KEY_FPS, 30);
    format.putInt32(HwAbsCodec::KEY_QUALITY, 15);
    if (Hw::SUCCESS != vCodec->configure(&format)) {
        Logcat::e("HWVC", "HwAndroidEncoder::configure failed to configure video codec!");
        release();
        return false;
    }
    muxer = new HwFFMuxer();
    if (Hw::SUCCESS != muxer->configure(path, HwAbsMuxer::TYPE_MP4)) {
        Logcat::e("HWVC", "HwAndroidEncoder::configure failed to configure muxer!");
        release();
        return false;
    }
    /**
     * Muxer start
     */
//    aTrack = muxer->addTrack(codec);
//    if (HwAbsMuxer::TRACK_NONE == aTrack) {
//        Logcat::e("HWVC", "HwFFmpegEncoder::initialize failed to add audio track!");
//        release();
//        return false;
//    }
    vTrack = muxer->addTrack(vCodec);
    if (HwAbsMuxer::TRACK_NONE == vTrack) {
        Logcat::e("HWVC", "HwAndroidEncoder::configure failed to add video track!");
        release();
        return false;
    }
    if (Hw::SUCCESS != muxer->start()) {
        Logcat::e("HWVC", "HwAndroidEncoder::configure failed to start muxer!");
        release();
        return false;
    }
    return true;
}

HwResult HwAndroidEncoder::write(HwAbsMediaFrame *frame) {
    lock_guard<std::mutex> guard(lock);
    AVPacket *packet = nullptr;
    if (frame->isVideo() && vCodec && muxer) {
        vCodec->encode(frame, reinterpret_cast<void **>(&packet));
        if (packet) {
            muxer->write(vTrack, packet);
        }
        return Hw::SUCCESS;
    }
    return Hw::FAILED;
}

bool HwAndroidEncoder::stop() {
    return true;
}

void HwAndroidEncoder::release() {
    if (vCodec) {
        delete vCodec;
        vCodec = nullptr;
    }
    if (muxer) {
        delete muxer;
        muxer = nullptr;
    }
    aTrack = HwAbsMuxer::TRACK_NONE;
    vTrack = HwAbsMuxer::TRACK_NONE;
}

void HwAndroidEncoder::flush() {
//    push(nullptr);
//    while (Hw::SUCCESS == pop()) {
//
//    }
}
