/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "HwAndroidEncoder.h"
#include "Logcat.h"
#include "../core/HwAndroidCodec.h"
#include "../../../include/HwFFMuxer.h"
#include "../../../include/HwFFCodec.h"

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
    const int32_t fps = 30;
    HwBundle format;
    format.putInt32(HwAbsCodec::KEY_FORMAT, static_cast<int32_t>(HwFrameFormat::HW_IMAGE_YV12));
    format.putInt32(HwAbsCodec::KEY_WIDTH, width);
    format.putInt32(HwAbsCodec::KEY_HEIGHT, height);
    format.putInt32(HwAbsCodec::KEY_BIT_RATE, width * height * 3);
    format.putInt32(HwAbsCodec::KEY_FPS, fps);
    format.putInt32(HwAbsCodec::KEY_QUALITY, 15);
    if (Hw::SUCCESS != vCodec->configure(&format)) {
        Logcat::e("HWVC", "HwAndroidEncoder::configure failed to configure video codec!");
        release();
        return false;
    }
    /**
     * Audio codec
     */
    HwBundle aBundle;
    aBundle.putInt32(HwAbsCodec::KEY_SAMPLE_RATE, audioFormat.getSampleRate());
    aBundle.putInt32(HwAbsCodec::KEY_CHANNELS, audioFormat.getChannels());
    aBundle.putInt32(HwAbsCodec::KEY_FORMAT, static_cast<int32_t>(audioFormat.getFormat()));
    aBundle.putInt32(HwAbsCodec::KEY_BIT_RATE, 64000);
    aCodec = new HwFFCodec(AV_CODEC_ID_AAC);
    if (Hw::SUCCESS != aCodec->configure(&aBundle)) {
        Logcat::e("HWVC", "HwFFmpegEncoder::initialize failed to configure audio codec!");
        release();
        return false;
    }
    aCodec->start();
    muxer = new HwFFMuxer();
    if (Hw::SUCCESS != muxer->configure(path, HwAbsMuxer::TYPE_MP4)) {
        Logcat::e("HWVC", "HwAndroidEncoder::configure failed to configure muxer!");
        release();
        return false;
    }
    /**
     * Muxer start
     */
    vTrack = muxer->addTrack(vCodec);
    if (HwAbsMuxer::TRACK_NONE == vTrack) {
        Logcat::e("HWVC", "HwAndroidEncoder::configure failed to add video track!");
        release();
        return false;
    }
    aTrack = muxer->addTrack(aCodec);
    if (HwAbsMuxer::TRACK_NONE == aTrack) {
        Logcat::e("HWVC", "HwFFmpegEncoder::initialize failed to add audio track!");
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
    HwPacket *packet = nullptr;
    if (frame->isAudio() && aCodec && muxer) {
        // Ensure that the first frame is video.
        if (!firstVideoFrameWrite) {
            return Hw::FAILED;
        }
        aCodec->process(&frame, &packet);
        if (packet) {
            muxer->write(aTrack, packet);
        }
        return Hw::SUCCESS;
    } else if (frame->isVideo() && vCodec && muxer) {
        vCodec->process(&frame, &packet);
        if (packet) {
            muxer->write(vTrack, packet);
            firstVideoFrameWrite = true;
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
