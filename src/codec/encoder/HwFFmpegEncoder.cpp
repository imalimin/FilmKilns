/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwFFmpegEncoder.h"
#include "Logcat.h"
#include "../include/HwVideoFrame.h"
#include "../include/HwAudioFrame.h"
#include "../include/HwFFMuxer.h"
#include "../include/HwFFCodec.h"
#include "TimeUtils.h"

#define TAG "HwFFmpegEncoder"

HwFFmpegEncoder::HwFFmpegEncoder() : HwAbsVideoEncoder() {

}

HwFFmpegEncoder::~HwFFmpegEncoder() {
    release();
}

bool HwFFmpegEncoder::prepare(string path, int width, int height, HwSampleFormat audioFormat) {
    this->path = path;
    this->width = width;
    this->height = height;
    this->audioFormat = audioFormat;
    return initialize();
}

bool HwFFmpegEncoder::initialize() {
    av_register_all();
    /**
     * Video codec
     */
    const int32_t fps = 30;
    HwBundle bundle;
    bundle.putInt32(HwAbsCodec::KEY_FORMAT, static_cast<int32_t>(HwFrameFormat::HW_IMAGE_YV12));
    bundle.putInt32(HwAbsCodec::KEY_WIDTH, width);
    bundle.putInt32(HwAbsCodec::KEY_HEIGHT, height);
    bundle.putInt32(HwAbsCodec::KEY_BIT_RATE, width * height * 3);
    bundle.putInt32(HwAbsCodec::KEY_FPS, fps);
    //For CRF
    bundle.putInt32(HwAbsCodec::KEY_QUALITY, quality);
    vCodec = new HwFFCodec(AV_CODEC_ID_H264);
    if (Hw::SUCCESS != vCodec->configure(bundle)) {
        AlLogE(TAG, "failed to configure video codec!");
        release();
        return false;
    }
    vCodec->start();
    if (audioFormat.valid()) {
        /**
         * Audio codec
         */
        HwBundle aBundle;
        aBundle.putInt32(HwAbsCodec::KEY_SAMPLE_RATE, audioFormat.getSampleRate());
        aBundle.putInt32(HwAbsCodec::KEY_CHANNELS, audioFormat.getChannels());
        aBundle.putInt32(HwAbsCodec::KEY_FORMAT, static_cast<int32_t>(audioFormat.getFormat()));
        aBundle.putInt32(HwAbsCodec::KEY_BIT_RATE, 64000);
        aCodec = new HwFFCodec(AV_CODEC_ID_AAC);
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

HwResult HwFFmpegEncoder::write(HwAbsMediaFrame *frame) {
    lock_guard<std::mutex> guard(lock);
    HwPacket *packet = nullptr;
    if (frame->isAudio() && aCodec && muxer) {
        // Ensure that the first frame is video.
        if (!firstVideoFrameWrite) {
            return Hw::FAILED;
        }
//        sampleCount += aCodec->getFrameSize();
        aCodec->process(&frame, &packet);
        if (packet) {
            muxer->write(aTrack, packet);
        }
        return Hw::SUCCESS;
    } else if (frame->isVideo() && vCodec && muxer) {
        int64_t time = TimeUtils::getCurrentTimeUS();
        if (lastTime > 0) {
            countOfTime += (time - lastTime);
            ++countOfFrame;
            if (countOfFrame >= 100) {
                AlLogI(TAG, "fps %lld", (countOfFrame * 1000000 / countOfTime));
                countOfTime = 0;
                countOfFrame = 0;
            }
        }
        lastTime = time;
        vCodec->process(&frame, &packet);
        if (packet) {
            muxer->write(vTrack, packet);
            firstVideoFrameWrite = true;
        }
        return Hw::SUCCESS;
    }
    return Hw::FAILED;
}

bool HwFFmpegEncoder::stop() {
//    int ret;
//    int got_picture;
//    if (!(pVideoStream->codec->codec->capabilities & AV_CODEC_CAP_DELAY)) {
//        return true;
//    }
//    while (1) {
//        avPacket->data = NULL;
//        avPacket->size = 0;
//        av_init_packet(avPacket);
//        ret = avcodec_encode_video2(pVideoStream->codec, avPacket,
//                                    NULL, &got_picture);
//        av_packet_unref(avPacket);
//        if (ret < 0)
//            break;
//        if (!got_picture) {
//            break;
//        }
//        ret = av_write_frame(pFormatCtx, avPacket);
//        if (ret < 0)
//            break;
//    }
    return true;
}

void HwFFmpegEncoder::release() {
    if (aCodec) {
        delete aCodec;
        aCodec = nullptr;
    }
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