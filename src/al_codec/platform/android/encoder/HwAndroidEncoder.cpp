/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "HwAndroidEncoder.h"
#include "Logcat.h"
#include "../core/HwAndroidCodec.h"
#include "../core/AlAndroidCodecCompat.h"
#include "../core/AlAndroidCodecCompat2.h"
#include "HwFFMuxer.h"
#include "HwFFCodec.h"
#include "TimeUtils.h"

#define TAG "HwAndroidEncoder"

HwAndroidEncoder::HwAndroidEncoder(const AlAbsEncoder::Desc &desc) : AlAbsVideoEncoder(desc) {

}

HwAndroidEncoder::~HwAndroidEncoder() {
    release();
}

void HwAndroidEncoder::setBitrate(int32_t rate) {

}

void HwAndroidEncoder::setProfile(std::string profile) {

}

void HwAndroidEncoder::setPreset(std::string preset) {

}

bool HwAndroidEncoder::prepare(string path, int width, int height, HwSampleFormat audioFormat) {
    this->path = path;
    this->width = width;
    this->height = height;
    this->audioFormat = audioFormat;
    return configure();
}

bool HwAndroidEncoder::configure() {
    if (AlAbsEncoder::kType::HARD == getCodecDesc().type) {
        vCodec = new AlAndroidCodecCompat(getCodecDesc().vID);
    } else if (AlAbsEncoder::kType::HARD_ENC_TEX == getCodecDesc().type) {
        vCodec = new AlAndroidCodecCompat2(getCodecDesc().vID);
    } else {
        AlLogE(TAG, "failed. Not support this encoder type.");
        release();
        return false;
    }
    const int32_t fps = 30;
    HwBundle format;
    format.putInt32(AlCodec::KEY_FORMAT, static_cast<int32_t>(HwFrameFormat::HW_IMAGE_YV12));
    format.putInt32(AlCodec::KEY_WIDTH, width);
    format.putInt32(AlCodec::KEY_HEIGHT, height);
    format.putInt32(AlCodec::KEY_BIT_RATE, width * height * 3);
    format.putInt32(AlCodec::KEY_FPS, fps);
    format.putInt32(AlCodec::KEY_QUALITY, 15);
    if (Hw::SUCCESS != vCodec->configure(format)) {
        AlLogE(TAG, "failed to configure video codec!");
        release();
        return false;
    }
    /**
     * Audio codec
     */
    if (audioFormat.valid()) {
        HwBundle aBundle;
        aBundle.putInt32(AlCodec::KEY_SAMPLE_RATE, audioFormat.getSampleRate());
        aBundle.putInt32(AlCodec::KEY_CHANNELS, audioFormat.getChannels());
        aBundle.putInt32(AlCodec::KEY_FORMAT, static_cast<int32_t>(audioFormat.getFormat()));
        aBundle.putInt32(AlCodec::KEY_BIT_RATE, 64000);
        aCodec = new HwFFCodec(getCodecDesc().aID);
        if (Hw::SUCCESS != aCodec->configure(aBundle)) {
            AlLogE(TAG, "failed to configure audio codec!");
            release();
            return false;
        }
        aCodec->start();
    } else {
        AlLogW(TAG, "Without audio track.");
    }
    muxer = new HwFFMuxer();
    if (Hw::SUCCESS != muxer->configure(path, HwAbsMuxer::TYPE_MP4)) {
        AlLogE(TAG, "failed to configure muxer!");
        release();
        return false;
    }
    /**
     * Muxer start
     */
    vTrack = muxer->addTrack(vCodec);
    if (HwAbsMuxer::TRACK_NONE == vTrack) {
        AlLogE(TAG, "failed to add video track!");
        release();
        return false;
    }
    if (aCodec) {
        aTrack = muxer->addTrack(aCodec);
        if (HwAbsMuxer::TRACK_NONE == aTrack) {
            AlLogE(TAG, "failed to add audio track!");
            release();
            return false;
        }
    }
    if (Hw::SUCCESS != muxer->start()) {
        AlLogE(TAG, "failed to start muxer!");
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
        fps.record(TAG);
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
    delete aCodec;
    aCodec = nullptr;
    delete vCodec;
    vCodec = nullptr;
    delete muxer;
    muxer = nullptr;
    aTrack = HwAbsMuxer::TRACK_NONE;
    vTrack = HwAbsMuxer::TRACK_NONE;
}

void HwAndroidEncoder::flush() {
//    push(nullptr);
//    while (Hw::SUCCESS == pop()) {
//
//    }
}
