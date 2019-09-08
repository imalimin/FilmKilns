/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwVideoUtils.h"
#include "Logcat.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "../include/ff/libavcodec/avcodec.h"
#include "../include/ff/libavformat/avformat.h"
#include "../include/ff/libavutil/avutil.h"
#include "../include/ff/libswresample/swresample.h"

#ifdef __cplusplus
}
#endif

HwResult HwVideoUtils::remux(std::string input, std::string output,
                             std::vector<int64_t> trimIns,
                             std::vector<int64_t> trimOuts) {
    av_register_all();
    /**
     * Open input
     */
    AVFormatContext *ctx = avformat_alloc_context();
    if (avformat_open_input(&ctx, input.c_str(), NULL, NULL) != 0) {
        Logcat::e("hwvc", "Couldn't open input stream.");
        return Hw::FAILED;
    }
    if (avformat_find_stream_info(ctx, NULL) < 0) {
        Logcat::e("hwvc", "Couldn't find stream information.");
        return Hw::FAILED;
    }
    int32_t videoTrack = -1, audioTrack = -1;
    for (int i = 0; i < ctx->nb_streams; i++) {
        if (-1 == videoTrack &&
            AVMediaType::AVMEDIA_TYPE_VIDEO == ctx->streams[i]->codecpar->codec_type) {
            videoTrack = i;
        }
        if (-1 == audioTrack &&
            AVMediaType::AVMEDIA_TYPE_AUDIO == ctx->streams[i]->codecpar->codec_type) {
            audioTrack = i;
        }
    }
    /**
     * Open output
     */
    int32_t ovideoTrack = -1, oaudioTrack = -1;
    AVFormatContext *octx = nullptr;
    int ret = avformat_alloc_output_context2(&octx, NULL, "MP4", output.c_str());
    if (ret < 0 || !octx) {
        Logcat::e("HWVC", "HwFFMuxer::configure failed %s", strerror(AVUNERROR(ret)));
        return Hw::FAILED;
    }
    av_dict_set(&octx->metadata, "comment", "hwvc", 0);
    if (videoTrack >= 0) {
        AVStream *stream = avformat_new_stream(octx, nullptr);
        avcodec_parameters_copy(stream->codecpar, ctx->streams[videoTrack]->codecpar);
        ovideoTrack = stream->index;
    }
    if (audioTrack >= 0) {
        AVStream *stream = avformat_new_stream(octx, nullptr);
        avcodec_parameters_copy(stream->codecpar, ctx->streams[audioTrack]->codecpar);
        oaudioTrack = stream->index;
    }
    if (avio_open2(&octx->pb, output.c_str(), AVIO_FLAG_WRITE, nullptr, nullptr) < 0) {
        return Hw::FAILED;
    }
    ret = avformat_write_header(octx, nullptr);
    /*
     * Start remxuer.
     */
    AVPacket *avPacket = av_packet_alloc();
    ret = 0;
    bool reset = false;
    int64_t aTime = 0, vTime = 0;
    int64_t aDTime = 0, vDTime = 0;
    int64_t aLastTime = 0, vLastTime = 0;
    int64_t aDLastTime = 0, vDLastTime = 0;
    while (AVERROR_EOF != ret) {
        ret = av_read_frame(ctx, avPacket);
        if (0 == ret) {
            int64_t pts = av_rescale_q_rnd(avPacket->pts,
                                           ctx->streams[avPacket->stream_index]->time_base,
                                           AV_TIME_BASE_Q,
                                           AV_ROUND_NEAR_INF);
            if (!contains(&trimIns, &trimOuts, pts)) {
                if (videoTrack == avPacket->stream_index) {
                    vLastTime = avPacket->pts;
                    vDLastTime = avPacket->dts;
                } else if (audioTrack == avPacket->stream_index) {
                    aLastTime = avPacket->pts;
                    aDLastTime = avPacket->dts;
                }
                av_packet_unref(avPacket);
                continue;
            }
            if (videoTrack == avPacket->stream_index) {
//                Logcat::i("hwvc", "HwFFmpegEncoder::write %d", avPacket->flags & AV_PKT_FLAG_KEY);
                vTime += (avPacket->pts - vLastTime);
                vDTime += (avPacket->dts - vDLastTime);
                vLastTime = avPacket->pts;
                vDLastTime = avPacket->dts;
                avPacket->pts = vTime;
                avPacket->dts = vDTime;
                avPacket->stream_index = ovideoTrack;
            } else if (audioTrack == avPacket->stream_index) {
                aTime += (avPacket->pts - aLastTime);
                aDTime += (avPacket->dts - aDLastTime);
                aLastTime = avPacket->pts;
                aDLastTime = avPacket->dts;
                avPacket->pts = aTime;
                avPacket->dts = aDTime;
                avPacket->stream_index = oaudioTrack;
            } else {
                av_packet_unref(avPacket);
                continue;
            }
            int flag = av_interleaved_write_frame(octx, avPacket);
            av_packet_unref(avPacket);
            Logcat::i("hwvc", "remuxer end. write %lld, ret = %d", pts, flag);
        }
    }
    /*
     * Release output
     */
    if (octx) {
        av_write_trailer(octx);
        if (!(octx->flags & AVFMT_NOFILE)) {
            avio_closep(&octx->pb);
        }
        avformat_free_context(octx);
        octx = nullptr;
    }
    Logcat::i("hwvc", "remuxer end.");
    if (ctx) {
        if (!(ctx->flags & AVFMT_NOFILE)) {
            avio_closep(&ctx->pb);
        }
        avformat_free_context(ctx);
        ctx = nullptr;
    }
    if (avPacket) {
        av_packet_free(&avPacket);
        avPacket = nullptr;
    }
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