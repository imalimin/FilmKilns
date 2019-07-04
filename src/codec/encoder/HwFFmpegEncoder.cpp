/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwFFmpegEncoder.h"
#include "Logcat.h"
#include "../include/HwVideoFrame.h"

HwFFmpegEncoder::HwFFmpegEncoder() : HwAbsEncoder() {

}

HwFFmpegEncoder::~HwFFmpegEncoder() {
    release();
}

bool HwFFmpegEncoder::prepare(string path, int width, int height) {
    this->path = path;
    this->width = width;
    this->height = height;
    return initialize();
}

bool HwFFmpegEncoder::initialize() {
    av_register_all();
    avformat_alloc_output_context2(&pFormatCtx, NULL, "mp4", path.c_str());
    pFormatCtx->oformat->video_codec = AV_CODEC_ID_H264;
    if (avio_open2(&pFormatCtx->pb, path.c_str(), AVIO_FLAG_WRITE, nullptr, nullptr) < 0) {
        avformat_free_context(pFormatCtx);
        pFormatCtx = nullptr;
        Logcat::e("HWVC", "HwFFmpegEncoder::initialize failed to open output file!");
        return false;
    }
    av_dict_set(&pFormatCtx->metadata, "comment", "hwvc", 0);
    pVideoStream = avformat_new_stream(pFormatCtx, 0);
    if (nullptr == pVideoStream) {
        release();
        Logcat::e("HWVC", "HwFFmpegEncoder::initialize failed to create video stream!");
        return false;
    }
    pVideoStream->time_base = outTimeBase;
    AVCodecContext *pCodecCtx = pVideoStream->codec;
    configure(pCodecCtx);
    /**
     * Important.If not set this, the output will be fail.
     */
    if (pFormatCtx->oformat->flags & AVFMT_GLOBALHEADER) {
        pVideoStream->codec->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }
    AVCodec *pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
    if (!pCodec) {
        release();
        Logcat::e("HWVC", "HwFFmpegEncoder::initialize could not find %d codec!",
                  pCodecCtx->codec_id);
        return false;
    }
    AVDictionary *param = nullptr;
    if (AV_CODEC_ID_H264 == pCodecCtx->codec_id) {
        av_dict_set(&param, "preset", "superfast", 0);
        av_dict_set(&param, "tune", "zerolatency", 0);
        av_dict_set(&param, "crf", "15", 0);
        //av_dict_set(param, "profile", "main", 0)
    }
    if (avcodec_open2(pCodecCtx, pCodec, &param) < 0) {
        release();
        Logcat::e("HWVC", "HwFFmpegEncoder::initialize could not open %d codec!",
                  pCodecCtx->codec_id);
        return false;
    }
    avformat_write_header(pFormatCtx, nullptr);
    avFrame = av_frame_alloc();
    avPacket = av_packet_alloc();
    return true;
}

void HwFFmpegEncoder::configure(AVCodecContext *ctx) {
    ctx->codec_id = pFormatCtx->oformat->video_codec;
    ctx->codec_type = AVMEDIA_TYPE_VIDEO;
    ctx->pix_fmt = AV_PIX_FMT_YUV420P;
    ctx->width = width;
    ctx->height = height;
    ctx->bit_rate = width * height * 3;
    ctx->profile = FF_PROFILE_H264_HIGH;
    ctx->gop_size = 150;

    ctx->time_base = outTimeBase;

    ctx->thread_count = 0;
    ctx->qmin = 10;
    ctx->qmax = 30;
    ctx->max_b_frames = 3;
}

HwResult HwFFmpegEncoder::write(HwAbsMediaFrame *frame) {
    if (!frame->isVideo()) {
        Logcat::e("HWVC", "HwFFmpegEncoder::encode failed!");
        return Hw::FAILED;
    }
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
    avFrame->pts = frame->getPts();
    avFrame->format = HwAbsMediaFrame::convertVideoFrameFormat(frame->getFormat());

    avcodec_send_frame(pVideoStream->codec, avFrame);
    int ret = avcodec_receive_packet(pVideoStream->codec, avPacket);
    if (ret < 0) {
        Logcat::e("HWVC", "HwFFmpegEncoder::encode failed!");
        return Hw::FAILED;
    }
    avPacket->stream_index = pVideoStream->index;
    avPacket->duration = 1;
    ret = av_write_frame(pFormatCtx, avPacket);
    if (0 == ret) {
        av_packet_unref(avPacket);
        return Hw::SUCCESS;
    }
    av_packet_unref(avPacket);
    return Hw::FAILED;
}

bool HwFFmpegEncoder::stop() {
    int ret;
    int got_picture;
    if (!(pVideoStream->codec->codec->capabilities & AV_CODEC_CAP_DELAY)) {
        return true;
    }
    while (1) {
        avPacket->data = NULL;
        avPacket->size = 0;
        av_init_packet(avPacket);
        ret = avcodec_encode_video2(pVideoStream->codec, avPacket,
                                    NULL, &got_picture);
        av_packet_unref(avPacket);
        if (ret < 0)
            break;
        if (!got_picture) {
            break;
        }
        ret = av_write_frame(pFormatCtx, avPacket);
        if (ret < 0)
            break;
    }
    return true;
}

void HwFFmpegEncoder::release() {
    if (pFormatCtx) {
        av_write_trailer(pFormatCtx);
    }
    if (avPacket) {
        av_packet_free(&avPacket);
        avPacket = nullptr;
    }
    if (avFrame) {
        av_frame_free(&avFrame);
        avFrame = nullptr;
    }
    if (pVideoStream && pVideoStream->codec) {
        avcodec_close(pVideoStream->codec);
        pVideoStream->codec = nullptr;
        pVideoStream = nullptr;
    }
    if (pFormatCtx) {
        avio_close(pFormatCtx->pb);
        avformat_free_context(pFormatCtx);
        pFormatCtx = nullptr;
    }
}