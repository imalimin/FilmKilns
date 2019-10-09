/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwVideoUtils.h"
#include "Logcat.h"

HwVideoUtils::Context::Context() : Object() {

}

HwVideoUtils::Context::~Context() {
    if (c) {
        if (!(c->flags & AVFMT_NOFILE) && c->pb) {
            avio_closep(&c->pb);
        }
        avformat_free_context(c);
        c = nullptr;
    }
}

HwVideoUtils::Context *HwVideoUtils::Context::open(std::string path) {
    HwVideoUtils::Context *ctx = new HwVideoUtils::Context();
    ctx->c = avformat_alloc_context();
    if (avformat_open_input(&(ctx->c), path.c_str(), NULL, NULL) != 0) {
        Logcat::e("hwvc", "HwVideoUtils::remux Couldn't open input stream.");
        delete ctx;
        return nullptr;
    }
    if (avformat_find_stream_info(ctx->c, NULL) < 0) {
        Logcat::e("hwvc", "HwVideoUtils::remux Couldn't find stream information.");
        delete ctx;
        return nullptr;
    }
    for (int i = 0; i < ctx->c->nb_streams; i++) {
        if (-1 == ctx->vTrackIndex &&
            AVMediaType::AVMEDIA_TYPE_VIDEO == ctx->c->streams[i]->codecpar->codec_type) {
            ctx->vTrackIndex = i;
        }
        if (-1 == ctx->aTrackIndex &&
            AVMediaType::AVMEDIA_TYPE_AUDIO == ctx->c->streams[i]->codecpar->codec_type) {
            ctx->aTrackIndex = i;
        }
    }
    return ctx;
}

HwVideoUtils::Context *HwVideoUtils::Context::open(std::string path,
                                                   HwVideoUtils::Context *iCtx) {
    HwVideoUtils::Context *ctx = new HwVideoUtils::Context();
    int ret = avformat_alloc_output_context2(&(ctx->c), NULL, "MP4", path.c_str());
    if (ret < 0 || !ctx->c) {
        Logcat::e("HWVC", "HwVideoUtils::remux configure failed %s", strerror(AVUNERROR(ret)));
        delete ctx;
        return nullptr;
    }
    av_dict_set(&ctx->c->metadata, "comment", "hwvc", 0);
    if (iCtx->vTrackIndex >= 0) {
        AVStream *stream = avformat_new_stream(ctx->c, nullptr);
        avcodec_parameters_copy(stream->codecpar, iCtx->c->streams[iCtx->vTrackIndex]->codecpar);
        ctx->vTrackIndex = stream->index;
    }
    if (iCtx->aTrackIndex >= 0) {
        AVStream *stream = avformat_new_stream(ctx->c, nullptr);
        avcodec_parameters_copy(stream->codecpar, iCtx->c->streams[iCtx->aTrackIndex]->codecpar);
        ctx->aTrackIndex = stream->index;
    }
    if (avio_open2(&ctx->c->pb, path.c_str(), AVIO_FLAG_WRITE, nullptr, nullptr) < 0) {
        delete ctx;
        return nullptr;
    }
    ret = avformat_write_header(ctx->c, nullptr);
    return ctx;
}

bool HwVideoUtils::recode(AVPacket **pkt, Context *iCtx, RecodeContext *rCtx) {
    if (RecodeContext::FLAG_LAST_PKT_SKIPPED == rCtx->flag && !((*pkt)->flags & AV_PKT_FLAG_KEY)) {
        rCtx->flag = RecodeContext::FLAG_RECODING;
        if (nullptr == rCtx->dCtx && nullptr == rCtx->eCtx) {
            createCodec(iCtx->c->streams[iCtx->vTrackIndex], &rCtx->dCtx, &rCtx->eCtx);
        }
    }
    if (RecodeContext::FLAG_RECODING == rCtx->flag && (*pkt)->flags & AV_PKT_FLAG_KEY) {
        rCtx->flag = RecodeContext::FLAG_NONE;
    }
    if (RecodeContext::FLAG_RECODING == rCtx->flag) {

    }
    return true;
}

HwResult HwVideoUtils::remux(std::string input, std::string output,
                             std::vector<int64_t> trimIns,
                             std::vector<int64_t> trimOuts) {
    av_register_all();
    /**
     * Open input
     */
    HwVideoUtils::Context *iCtx = HwVideoUtils::Context::open(input);
    if (nullptr == iCtx) {
        return Hw::FAILED;
    }
    /**
     * Open output
     */
    HwVideoUtils::Context *oCtx = HwVideoUtils::Context::open(output, iCtx);
    if (nullptr == oCtx) {
        return Hw::FAILED;
    }
    /*
     * Start remxuer.
     */
    AVPacket *avPacket = av_packet_alloc();
    auto *rCtx = new RecodeContext();
    int32_t ret = 0;
    int64_t aTime = 0, vTime = 0;
    int64_t aDTime = 0, vDTime = 0;
    int64_t aLastTime = 0, vLastTime = 0;
    int64_t aDLastTime = 0, vDLastTime = 0;
    while (AVERROR_EOF != ret) {
        ret = av_read_frame(iCtx->c, avPacket);
        if (0 == ret) {
            int64_t pts = av_rescale_q_rnd(avPacket->pts,
                                           iCtx->c->streams[avPacket->stream_index]->time_base,
                                           AV_TIME_BASE_Q,
                                           AV_ROUND_NEAR_INF);
            if (!contains(&trimIns, &trimOuts, pts)) {
                if (iCtx->vTrackIndex == avPacket->stream_index) {
                    if (1 == avPacket->flags & AV_PKT_FLAG_KEY) {
                        ++rCtx->gopIndex;
                    }
                    rCtx->flag = RecodeContext::FLAG_LAST_PKT_SKIPPED;
                    vLastTime = avPacket->pts;
                    vDLastTime = avPacket->dts;
                } else if (iCtx->aTrackIndex == avPacket->stream_index) {
                    aLastTime = avPacket->pts;
                    aDLastTime = avPacket->dts;
                }
                av_packet_unref(avPacket);
                continue;
            }
            if (iCtx->vTrackIndex == avPacket->stream_index) {
//                Logcat::i("hwvc", "HwFFmpegEncoder::write %d", avPacket->flags & AV_PKT_FLAG_KEY);
                recode(&avPacket, iCtx, rCtx);
                vTime += (avPacket->pts - vLastTime);
                vDTime += (avPacket->dts - vDLastTime);
                vLastTime = avPacket->pts;
                vDLastTime = avPacket->dts;
                avPacket->pts = vTime;
                avPacket->dts = vDTime;
                avPacket->stream_index = oCtx->vTrackIndex;
            } else if (iCtx->aTrackIndex == avPacket->stream_index) {
                aTime += (avPacket->pts - aLastTime);
                aDTime += (avPacket->dts - aDLastTime);
                aLastTime = avPacket->pts;
                aDLastTime = avPacket->dts;
                avPacket->pts = aTime;
                avPacket->dts = aDTime;
                avPacket->stream_index = oCtx->aTrackIndex;
            } else {
                av_packet_unref(avPacket);
                continue;
            }
            int flag = av_interleaved_write_frame(oCtx->c, avPacket);
            av_packet_unref(avPacket);
            Logcat::i("hwvc", "HwVideoUtils::remux end. write %lld, ret = %d", pts, flag);
        }
    }
    /*
     * Release output
     */
    av_write_trailer(oCtx->c);
    delete oCtx;
    delete iCtx;
    delete rCtx;
    if (avPacket) {
        av_packet_free(&avPacket);
        avPacket = nullptr;
    }
    Logcat::i("hwvc", "remuxer end.");
    return Hw::SUCCESS;
}

bool HwVideoUtils::contains(std::vector<int64_t> *trimIns, std::vector<int64_t> *trimOuts,
                            int64_t pts) {
    for (int i = 0; i < trimIns->size(); ++i) {
        if (pts >= (*trimIns)[i] && pts < (*trimOuts)[i]) {
            return true;
        }
    }
    return false;
}

bool HwVideoUtils::createCodec(AVStream *stream, AVCodecContext **dCtx, AVCodecContext **eCtx) {
    AVCodecParameters *avCodecParameters = stream->codecpar;
    AVCodec *codec = avcodec_find_decoder(avCodecParameters->codec_id);
    if (nullptr == codec) {
        return false;
    }
    *dCtx = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(*dCtx, avCodecParameters);
    if (avcodec_open2(*dCtx, codec, NULL) < 0) {
        Logcat::e("hwvc", "Couldn't open decode codec.");
        avcodec_free_context(dCtx);
        return false;
    }
    codec = avcodec_find_encoder(avCodecParameters->codec_id);
    *eCtx = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(*eCtx, avCodecParameters);
    if (avcodec_open2(*eCtx, codec, NULL) < 0) {
        Logcat::e("hwvc", "Couldn't open encode codec.");
        avcodec_close(*dCtx);
        avcodec_free_context(dCtx);
        avcodec_free_context(eCtx);
        return false;
    }
    return true;
}