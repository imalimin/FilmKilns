/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlFFEncoder.h"
#include "Logcat.h"
#include "HwVideoFrame.h"
#include "HwAudioFrame.h"
#include "HwFFMuxer.h"
#include "HwFFCodec.h"
#include "TimeUtils.h"
#include "AlFFUtils.h"

#define TAG "AlFFEncoder"

AlFFEncoder::AlFFEncoder(const AlAbsEncoder::Desc &desc) : AlAbsVideoEncoder(desc) {

}

AlFFEncoder::~AlFFEncoder() {
    release();
}

void AlFFEncoder::setBitrate(int32_t rate) {
    this->bitrate = rate;
}

void AlFFEncoder::setProfile(std::string profile) {
    this->profile = profile;
}

void AlFFEncoder::setPreset(std::string preset) {
    this->preset = preset;
}

bool AlFFEncoder::prepare(string path, int width, int height, HwSampleFormat audioFormat) {
    this->path = path;
    this->width = width;
    this->height = height;
    this->audioFormat = audioFormat;
    return initialize();
}

bool AlFFEncoder::initialize() {
    AlFFUtils::init();
    /**
     * Video codec
     */
    const int32_t fps = 30;
    AlBundle bundle;
    bundle.put(AlCodec::KEY_FORMAT, static_cast<int32_t>(HwFrameFormat::HW_IMAGE_YV12));
    bundle.put(AlCodec::KEY_WIDTH, width);
    bundle.put(AlCodec::KEY_HEIGHT, height);
    bundle.put(AlCodec::KEY_BIT_RATE, this->bitrate);
    bundle.put(AlCodec::KEY_FPS, fps);
    bundle.put(AlCodec::KEY_PROFILE, this->profile);
    bundle.put(AlCodec::KEY_PRESET, this->preset);
    //For CRF
    bundle.put(AlCodec::KEY_QUALITY, quality);
    vCodec = new HwFFCodec(getCodecDesc().vID);
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
        AlBundle aBundle;
        aBundle.put(AlCodec::KEY_SAMPLE_RATE, (int32_t) audioFormat.getSampleRate());
        aBundle.put(AlCodec::KEY_CHANNELS, audioFormat.getChannels());
        aBundle.put(AlCodec::KEY_FORMAT, static_cast<int32_t>(audioFormat.getFormat()));
        aBundle.put(AlCodec::KEY_BIT_RATE, 64000);
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
    AlLogI(TAG, vCodec->getFormat().toString());
    AlLogI(TAG, aCodec->getFormat().toString());
    muxer = new HwFFMuxer();
    if (Hw::SUCCESS != muxer->configure(path, HwAbsMuxer::TYPE_MP4)) {
        AlLogE(TAG, "failed to configure muxer!");
        release();
        return false;
    }
    /**
     * Muxer start
     */
    vTrack = muxer->addTrack(vCodec->getFormat());
    if (HwAbsMuxer::TRACK_NONE == vTrack) {
        AlLogE(TAG, "failed to add video track!");
        release();
        return false;
    }
    if (aCodec) {
        aTrack = muxer->addTrack(aCodec->getFormat());
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

HwResult AlFFEncoder::write(HwAbsMediaFrame *frame) {
    lock_guard<std::mutex> guard(lock);
    HwPacket *packet = nullptr;
    if (frame->isAudio() && aCodec && muxer) {
        // Ensure that the first frame is video.
//        sampleCount += aCodec->getFrameSize();
        if (!firstVideoFrameWrite) {
            return Hw::FAILED;
        }
        do {
            aCodec->process(&frame, &packet);
            if (packet) {
                muxer->write(aTrack, packet);
            }
        } while (packet && packet->getFlags() & AlMediaDef::FLAG_CONFIG);
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
        do {
            vCodec->process(&frame, &packet);
            if (packet) {
                muxer->write(vTrack, packet);
            }
        } while (packet && packet->getFlags() & AlMediaDef::FLAG_CONFIG);
        firstVideoFrameWrite = true;
        return Hw::SUCCESS;
    }
    return Hw::FAILED;
}

bool AlFFEncoder::stop() {
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

void AlFFEncoder::release() {
    delete aCodec;
    aCodec = nullptr;
    delete vCodec;
    vCodec = nullptr;
    delete muxer;
    muxer = nullptr;
    aTrack = HwAbsMuxer::TRACK_NONE;
    vTrack = HwAbsMuxer::TRACK_NONE;
}
