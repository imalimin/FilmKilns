/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwFFCodec.h"
#include "../include/HwAudioFrame.h"
#include "../include/HwVideoFrame.h"
#include "Logcat.h"

HwFFCodec::HwFFCodec(int32_t codecId) : HwAbsCodec(codecId) {

}

HwFFCodec::~HwFFCodec() {
    release();
}

void HwFFCodec::release() {
    if (avPacket) {
        av_packet_free(&avPacket);
        avPacket = nullptr;
    }
    if (avFrame) {
        av_frame_free(&avFrame);
        avFrame = nullptr;
    }
    if (ctx) {
        avcodec_close(ctx);
        avcodec_free_context(&ctx);
    }
    if (translator) {
        delete translator;
        translator = nullptr;
    }
}

HwResult HwFFCodec::configure(HwBundle *format) {
    HwAbsCodec::configure(format);
    if (codecId <= AV_CODEC_ID_NONE) {
        return Hw::FAILED;
    }
    AVCodecID id = static_cast<AVCodecID>(codecId);
    AVCodec *pCodec = avcodec_find_encoder(id);
    if (!pCodec) {
        release();
        Logcat::e("HWVC", "HwFFCodec::initialize could not find %d codec!", id);
        return Hw::FAILED;
    }
    ctx = avcodec_alloc_context3(pCodec);
    if (!ctx) {
        release();
        Logcat::e("HWVC", "HwFFCodec::initialize could not init %d codec context!", id);
        return Hw::FAILED;
    }
    switch (id) {
        case AV_CODEC_ID_H264: {
            if (!configureVideo(id, pCodec)) {
                Logcat::e("HWVC", "HwFFCodec::initialize could not open %d codec!", id);
                return Hw::FAILED;
            }
            break;
        }
        case AV_CODEC_ID_AAC_LATM:
        case AV_CODEC_ID_AAC: {
            if (!configureAudio(id, pCodec)) {
                Logcat::e("HWVC", "HwFFCodec::initialize could not open %d codec!", id);
                return Hw::FAILED;
            }
            break;
        }
        default:
            return Hw::FAILED;
    }
    //Copy parameters.
    if (format) {
        format->putInt32(KEY_PROFILE, ctx->profile);
        format->putInt32(KEY_LEVEL, ctx->level);
        format->putInt64(KEY_BIT_RATE, ctx->bit_rate);
        format->putInt32(KEY_FRAME_SIZE, ctx->frame_size);
    }
    avFrame = av_frame_alloc();
    avPacket = av_packet_alloc();
    if (ctx->codec_type == AVMEDIA_TYPE_AUDIO && format) {
        const int32_t &f = format->getInt32(KEY_FORMAT);
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
            (HwFrameFormat) format->getInt32(KEY_FORMAT));
    ctx->width = format->getInt32(KEY_WIDTH);
    ctx->height = format->getInt32(KEY_HEIGHT);
    ctx->bit_rate = ctx->width * ctx->height * 3;
    ctx->gop_size = 15;

    ctx->time_base = {1, format->getInt32(KEY_FPS)};

//    pCodecCtx->ticks_per_frame = 2;
    ctx->thread_count = 0;
    ctx->qmin = 10;
    ctx->qmax = 30;
    ctx->max_b_frames = 2;
//    pCodecCtx->me_range = 16;
//    pCodecCtx->max_qdiff = 4;
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
        av_dict_set_int(&param, "crf", format->getInt32(KEY_QUALITY), 0);  // or abr,qp
        av_dict_set(&param, "preset", "superfast", 0);
        //av_dict_set(param, "profile", "main", 0)
        av_dict_set(&param, "tune", "zerolatency", 0);
    }
    int ret = avcodec_open2(ctx, codec, &param);
    av_dict_free(&param);
    if (ret < 0) {
        release();
        Logcat::e("HWVC", "HwFFCodec::configureVideo could not open %d codec!",
                  ctx->codec_id);
        return false;
    }
    return true;
}

bool HwFFCodec::configureAudio(AVCodecID id, AVCodec *codec) {
    ctx->codec_id = id;
    ctx->codec_type = AVMEDIA_TYPE_AUDIO;
    ctx->sample_fmt = AV_SAMPLE_FMT_FLTP;
    ctx->bit_rate = 64000;
    ctx->sample_rate = format->getInt32(KEY_SAMPLE_RATE);
    ctx->channels = format->getInt32(KEY_CHANNELS);
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
        Logcat::e("HWVC", "HwFFCodec::configureAudio could not open audio codec: %s",
                  strerror(AVUNERROR(ret)));
        return false;
    }
    return true;
}

int32_t HwFFCodec::type() {
    switch (ctx->codec_type) {
        case AVMEDIA_TYPE_VIDEO: {
            return 0;
        }
        case AVMEDIA_TYPE_AUDIO: {
            return 1;
        }
        default: {
            return 0;
        }
    }
}

int32_t HwFFCodec::getExtraBuffer(string key, uint8_t **buf) {
    if (!ctx) {
        return 0;
    }
    //TODO simple now
    if ("csd-0" == key) {
        *buf = ctx->extradata;
        return ctx->extradata_size;
    }
    return 0;
}

HwResult HwFFCodec::encode(HwAbsMediaFrame *frame, void **packet) {
    switch (ctx->codec_type) {
        case AVMEDIA_TYPE_VIDEO: {
            HwVideoFrame *videoFrame = dynamic_cast<HwVideoFrame *>(frame);
            int pixelCount = videoFrame->getWidth() * videoFrame->getHeight();
            avFrame->data[0] = videoFrame->getBuffer()->getData();
            avFrame->data[1] = videoFrame->getBuffer()->getData() + pixelCount;
            avFrame->data[2] = videoFrame->getBuffer()->getData() + pixelCount * 5 / 4;

            avFrame->linesize[0] = videoFrame->getWidth();
            avFrame->linesize[1] = videoFrame->getWidth() / 2;
            avFrame->linesize[2] = videoFrame->getWidth() / 2;
            avFrame->width = videoFrame->getWidth();
            avFrame->height = videoFrame->getHeight();
            avFrame->format = HwAbsMediaFrame::convertVideoFrameFormat(frame->getFormat());
            avFrame->pts = frame->getPts();
            avFrame->pkt_duration = static_cast<int64_t>(AV_TIME_BASE /
                                                         (float) ctx->time_base.den );
//        avFrame->pts = frameCount;
//            avFrame->pts = av_rescale_q_rnd(frame->getPts(),
//                                            AV_TIME_BASE_Q,
//                                            pVideoStream->time_base,
//                                            AV_ROUND_NEAR_INF);
            avcodec_send_frame(ctx, avFrame);
            break;
        }
        case AVMEDIA_TYPE_AUDIO: {
            HwAudioFrame *audioFrame = dynamic_cast<HwAudioFrame *>(frame);
            int32_t frameSize = ctx->frame_size;
            if (ctx->codec->capabilities & AV_CODEC_CAP_VARIABLE_FRAME_SIZE) {
                frameSize = 10000;
            }
            avFrame->data[0] = audioFrame->getBuffer()->getData();
            avFrame->linesize[0] = audioFrame->getBufferSize();
            avFrame->format = HwAbsMediaFrame::convertAudioFrameFormat(frame->getFormat());
            avFrame->sample_rate = audioFrame->getSampleRate();
            avFrame->nb_samples = frameSize;
            avFrame->channels = audioFrame->getChannels();
            avFrame->channel_layout = static_cast<uint64_t>(av_get_default_channel_layout(
                    audioFrame->getChannels()));
            avFrame->pts = frame->getPts();
//        avFrame->pts = sampleCount;
//            avFrame->pts = av_rescale_q_rnd(frame->getPts(),
//                                                 AV_TIME_BASE_Q,
//                                                 pAudioStream->time_base,
//                                                 AV_ROUND_NEAR_INF);

            AVFrame *f = nullptr;
            translator->translate(&f, &avFrame);
            avFrame->pkt_duration = static_cast<int64_t>(AV_TIME_BASE / (float) ctx->sample_rate *
                                                         avFrame->nb_samples);
            avcodec_send_frame(ctx, f);
            break;
        }
        default:
            return Hw::FAILED;
    }
    av_packet_unref(avPacket);
    int ret = avcodec_receive_packet(ctx, avPacket);
    if (AVERROR(EAGAIN) == ret) {
        Logcat::e("HWVC", "HwFFCodec::encode wait(%s)", strerror(AVUNERROR(ret)));
        return Hw::FAILED;
    }
    if (ret < 0) {
        Logcat::e("HWVC", "HwFFCodec::encode failed: %s", strerror(AVUNERROR(ret)));
        return Hw::FAILED;
    }
    *packet = avPacket;
    return Hw::SUCCESS;
}