/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwFFCodec.h"
#include "../include/HwAudioFrame.h"
#include "../include/HwVideoFrame.h"
#include "BinaryUtils.h"
#include "Logcat.h"

#define TAG "HwFFCodec"

HwFFCodec::HwFFCodec(AlCodec::kID id) : AlCodec(id) {

}

HwFFCodec::~HwFFCodec() {
    release();
}

void HwFFCodec::release() {
    if (avPacket) {
        av_packet_free(&avPacket);
        avPacket = nullptr;
    }
    if (hwPacket) {
        delete hwPacket;
        hwPacket = nullptr;
    }
    if (avFrame) {
        av_frame_free(&avFrame);
        avFrame = nullptr;
    }
    if (ctx) {
        if (avcodec_is_open(ctx)) {
            avcodec_close(ctx);
        }
        avcodec_free_context(&ctx);
    }
    if (translator) {
        delete translator;
        translator = nullptr;
    }
    delete mExtraData;
    mExtraData = nullptr;
}

HwResult HwFFCodec::configure(AlBundle &format) {
    AlCodec::configure(format);
    if (getCodecID() == AlCodec::kID::NONE) {
        return Hw::FAILED;
    }
    AVCodecID id = static_cast<AVCodecID>(getCodecID());
    AVCodec *pCodec = avcodec_find_encoder(id);
    if (!pCodec) {
        AlLogE(TAG, "could not find %d codec!", id);
        release();
        return Hw::FAILED;
    }
    ctx = avcodec_alloc_context3(pCodec);
    if (!ctx) {
        AlLogE(TAG, "could not init %d codec context!", id);
        release();
        return Hw::FAILED;
    }
    switch (id) {
        case AV_CODEC_ID_H264: {
            if (!configureVideo(id, pCodec)) {
                AlLogE(TAG, "could not open %d codec!", id);
                return Hw::FAILED;
            }
            break;
        }
        case AV_CODEC_ID_GIF: {
            // Configure
            ctx->codec_id = id;
            ctx->codec_type = AVMEDIA_TYPE_VIDEO;
            ctx->pix_fmt = AV_PIX_FMT_PAL8;
            ctx->width = getFormat().get(KEY_WIDTH, INT32_MIN);
            ctx->height = getFormat().get(KEY_HEIGHT, INT32_MIN);
            ctx->time_base = {1, getFormat().get(KEY_FPS, INT32_MIN)};
            ctx->framerate = {getFormat().get(KEY_FPS, INT32_MIN), 1};
            _configureBitrate(getFormat().get(KEY_BIT_RATE, INT32_MIN));

            ctx->gop_size = 15;
            ctx->thread_count = 0;
            ctx->max_b_frames = 2;
            ctx->codec = pCodec;
            ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
            int len = ctx->extradata_size;
            int ret = avcodec_open2(ctx, pCodec, nullptr);
            if (ret < 0) {
                AlLogE(TAG, "could not open %d codec!", ctx->codec_id);
                release();
                return Hw::FAILED;
            }
            break;
        }
        case AV_CODEC_ID_AAC_LATM:
        case AV_CODEC_ID_AAC: {
            if (!configureAudio(id, pCodec)) {
                AlLogE(TAG, "could not open %d codec!", id);
                return Hw::FAILED;
            }
            break;
        }
        default:
            return Hw::FAILED;
    }
    if (ctx->extradata && ctx->extradata_size > 0) {
        delete mExtraData;
        mExtraData = AlBuffer::alloc(ctx->extradata_size);
        mExtraData->put(ctx->extradata, ctx->extradata_size);
        mExtraData->rewind();
        getFormat().put(KEY_EXTRA_DATA, (int64_t) mExtraData);
    }
    //Copy parameters.
    getFormat().remove(KEY_PROFILE);
    getFormat().put(KEY_PROFILE, ctx->profile);
    getFormat().put(KEY_LEVEL, ctx->level);
    getFormat().put(KEY_BIT_RATE, static_cast<int32_t>(ctx->bit_rate));
    getFormat().put(KEY_FRAME_SIZE, ctx->frame_size);

    avFrame = av_frame_alloc();
    avPacket = av_packet_alloc();
    if (ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
        const int32_t &f = format.get(KEY_FORMAT, INT32_MIN);
        translator = new HwAudioTranslator(HwSampleFormat(HwFrameFormat::HW_SAMPLE_FLTP,
                                                          ctx->channels,
                                                          ctx->sample_rate),
                                           HwSampleFormat((HwFrameFormat) f,
                                                          ctx->channels,
                                                          ctx->sample_rate));
    }
    return Hw::SUCCESS;
}

HwResult HwFFCodec::start() {
    return Hw::SUCCESS;
}

bool HwFFCodec::configureVideo(AVCodecID id, AVCodec *codec) {
    // Configure
    ctx->codec_id = id;
    ctx->codec_type = AVMEDIA_TYPE_VIDEO;
    ctx->pix_fmt = HwAbsMediaFrame::convertVideoFrameFormat(
            (HwFrameFormat) getFormat().get(KEY_FORMAT, INT32_MIN));
    ctx->width = getFormat().get(KEY_WIDTH, INT32_MIN);
    ctx->height = getFormat().get(KEY_HEIGHT, INT32_MIN);
    ctx->time_base = {1, getFormat().get(KEY_FPS, INT32_MIN)};
    ctx->framerate = {getFormat().get(KEY_FPS, INT32_MIN), 1};
    _configureBitrate(getFormat().get(KEY_BIT_RATE, INT32_MIN));

    ctx->gop_size = 15;
//    pCodecCtx->ticks_per_frame = 2;
    ctx->thread_count = 0;
//    ctx->qmin = 10;
//    ctx->qmax = 30;
    ctx->max_b_frames = 2;
//    ctx->me_range = 16;
//    ctx->max_qdiff = 4;
    ctx->codec = codec;
    /**
     * Important. If not set this, the output will be fail.
     */
//    if (pFormatCtx->oformat->flags & AVFMT_GLOBALHEADER) {
    ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
//    }
    AVDictionary *param = nullptr;
    if (AV_CODEC_ID_H264 == ctx->codec_id) {
        ctx->profile = FF_PROFILE_H264_HIGH;
        //0 - 51
//        av_dict_set_int(&param, "crf", getFormat().getInt32(KEY_QUALITY), 0);  // or abr,qp
        //ultrafast, superfast, veryfast, faster, fast, medium, slow, slower, veryslow and placebo.
        av_dict_set(&param, "preset", getFormat().get(AlCodec::KEY_PRESET, "").c_str(), 0);
        av_dict_set(&param, "profile", getFormat().get(AlCodec::KEY_PROFILE, "").c_str(), 0);
        av_dict_set(&param, "tune", "zerolatency", 0);
    }
    int ret = avcodec_open2(ctx, codec, &param);
    av_dict_free(&param);
    if (ret < 0) {
        AlLogE(TAG, "could not open %d codec!", ctx->codec_id);
        release();
        return false;
    }
    return true;
}

bool HwFFCodec::configureAudio(AVCodecID id, AVCodec *codec) {
    ctx->codec_id = id;
    ctx->codec_type = AVMEDIA_TYPE_AUDIO;
    ctx->sample_fmt = AV_SAMPLE_FMT_FLTP;
    ctx->bit_rate = getFormat().get(KEY_BIT_RATE, INT32_MIN);
    ctx->sample_rate = getFormat().get(KEY_SAMPLE_RATE, INT32_MIN);
    ctx->channels = getFormat().get(KEY_CHANNELS, INT32_MIN);
    ctx->channel_layout = static_cast<uint64_t>(
            av_get_default_channel_layout(ctx->channels));
    ctx->time_base = {1, ctx->sample_rate};
    ctx->codec = codec;
//    if (pFormatCtx->oformat->flags & AVFMT_GLOBALHEADER) {
    ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
//    }

    AVDictionary *param = nullptr;
    int ret = avcodec_open2(ctx, codec, &param);
    av_dict_free(&param);
    if (ret < 0) {
        if (ctx) {
            avcodec_close(ctx);
            avcodec_free_context(&ctx);
        }
        AlLogE(TAG, "could not open audio codec: %s", strerror(AVUNERROR(ret)));
        return false;
    }
    return true;
}

void HwFFCodec::_configureBitrate(int32_t bitrate) {
    ctx->bit_rate = bitrate;
}

AlCodec::kMediaType HwFFCodec::getMediaType() {
    switch (ctx->codec_type) {
        case AVMEDIA_TYPE_VIDEO: {
            return AlCodec::kMediaType::VIDEO;
        }
        case AVMEDIA_TYPE_AUDIO: {
            return AlCodec::kMediaType::AUDIO;
        }
        default: {
            return AlCodec::kMediaType::UNKNOWN;
        }
    }
}

void HwFFCodec::flush() {

}

HwResult HwFFCodec::process(HwAbsMediaFrame **frame, HwPacket **pkt) {
    int64_t duration = 1;
    switch (ctx->codec_type) {
        case AVMEDIA_TYPE_VIDEO: {
            HwVideoFrame *videoFrame = static_cast<HwVideoFrame *>(*frame);
            int pixelCount = videoFrame->getWidth() * videoFrame->getHeight();
            avFrame->data[0] = videoFrame->data();
            avFrame->data[1] = videoFrame->data() + pixelCount;
            avFrame->data[2] = videoFrame->data() + pixelCount * 5 / 4;

            avFrame->linesize[0] = videoFrame->getWidth();
            avFrame->linesize[1] = videoFrame->getWidth() / 2;
            avFrame->linesize[2] = videoFrame->getWidth() / 2;
            avFrame->width = videoFrame->getWidth();
            avFrame->height = videoFrame->getHeight();
            avFrame->format = HwAbsMediaFrame::convertVideoFrameFormat(videoFrame->getFormat());
            avFrame->pts = videoFrame->getPts();
            if (HwVideoFrame::HW_PIC_I == videoFrame->getPicType()) {
                avFrame->pict_type = AV_PICTURE_TYPE_I;
            } else {
                avFrame->pict_type = AV_PICTURE_TYPE_NONE;
            }
            duration = avFrame->pts - vLastPts;
            if (duration <= 0) {
                duration = static_cast<int64_t>(AV_TIME_BASE /
                                                (float) ctx->time_base.den );
            }
            vLastPts = avFrame->pts;
            int ret = avcodec_send_frame(ctx, avFrame);
            break;
        }
        case AVMEDIA_TYPE_AUDIO: {
            HwAudioFrame *audioFrame = static_cast<HwAudioFrame *>(*frame);
            int32_t frameSize = ctx->frame_size;
            if (ctx->codec->capabilities & AV_CODEC_CAP_VARIABLE_FRAME_SIZE) {
                frameSize = 10000;
            }
            avFrame->data[0] = audioFrame->data();
            avFrame->linesize[0] = audioFrame->size();
            avFrame->format = HwAbsMediaFrame::convertAudioFrameFormat(audioFrame->getFormat());
            avFrame->sample_rate = audioFrame->getSampleRate();
            avFrame->nb_samples = frameSize;
            avFrame->channels = audioFrame->getChannels();
            avFrame->channel_layout = static_cast<uint64_t>(av_get_default_channel_layout(
                    audioFrame->getChannels()));
            avFrame->pts = audioFrame->getPts();

            AVFrame *f = nullptr;
            translator->translate(&f, &avFrame);
            duration = static_cast<int64_t>(AV_TIME_BASE / (float) ctx->sample_rate *
                                            avFrame->nb_samples);
            int ret = avcodec_send_frame(ctx, f);
            break;
        }
        default:
            return Hw::FAILED;
    }
    av_packet_unref(avPacket);
    int ret = avcodec_receive_packet(ctx, avPacket);
    if (AVERROR(EAGAIN) == ret) {
        AlLogE(TAG, "wait(%s)", strerror(AVUNERROR(ret)));
        return Hw::FAILED;
    }
    if (ret < 0) {
        AlLogE(TAG, "failed: %s", strerror(AVUNERROR(ret)));
        return Hw::FAILED;
    }
    avPacket->duration = duration;
    if (hwPacket) {
        delete hwPacket;
    }
    hwPacket = HwPacket::wrap(avPacket);
    *pkt = hwPacket;
    return Hw::SUCCESS;
}
