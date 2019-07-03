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
    if (avio_open(&pFormatCtx->pb, path.c_str(), AVIO_FLAG_READ_WRITE) < 0) {
        avformat_free_context(pFormatCtx);
        pFormatCtx = nullptr;
        Logcat::e("HWVC", "HwFFmpegEncoder::initialize failed to open output file!");
        return false;
    }
    pVideoStream = avformat_new_stream(pFormatCtx, 0);
    if (nullptr == pVideoStream) {
        release();
        Logcat::e("HWVC", "HwFFmpegEncoder::initialize failed to create video stream!");
        return false;
    }
    pCodecCtx = pVideoStream->codec;
    pCodecCtx->codec_id = AV_CODEC_ID_H264;
    pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
    pCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
    pCodecCtx->width = width;
    pCodecCtx->height = height;
    pCodecCtx->bit_rate = 1024000;
    pCodecCtx->gop_size = 150;

    pCodecCtx->time_base.num = 1;
    pCodecCtx->time_base.den = 24;

    pCodecCtx->qmin = 10;
    pCodecCtx->qmax = 51;
    pCodecCtx->max_b_frames = 3;
    AVDictionary *param = nullptr;
    if (AV_CODEC_ID_H264 == pCodecCtx->codec_id) {
        av_dict_set(&param, "preset", "veryfast", 0);
        av_dict_set(&param, "tune", "zerolatency", 0);
        //av_dict_set(param, "profile", "main", 0)
    }
    pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
    if (!pCodec) {
        release();
        Logcat::e("HWVC", "HwFFmpegEncoder::initialize could not find %d codec!",
                  pCodecCtx->codec_id);
        return -1;
    }
    if (avcodec_open2(pCodecCtx, pCodec, &param) < 0) {
        release();
        Logcat::e("HWVC", "HwFFmpegEncoder::initialize could not open %d codec!",
                  pCodecCtx->codec_id);
        return -1;
    }
    avformat_write_header(pFormatCtx, NULL);
    avFrame = av_frame_alloc();
    int size = avpicture_get_size(pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height);
    uint8_t *buf = (uint8_t *) av_malloc(size);
    avpicture_fill((AVPicture *) avFrame, buf, pCodecCtx->pix_fmt, pCodecCtx->width,
                   pCodecCtx->height);
    av_new_packet(avPacket, size);
    return true;
}

HwResult HwFFmpegEncoder::encode(HwAbsMediaFrame *frame) {
    if (!frame->isVideo()) {
        Logcat::e("HWVC", "HwFFmpegEncoder::encode failed!");
        return Hw::FAILED;
    }
    int got_picture = 0;
    int ret = avcodec_encode_video2(pCodecCtx, avPacket, avFrame, &got_picture);
    if (ret < 0) {
        Logcat::e("HWVC", "HwFFmpegEncoder::encode failed!");
        return Hw::FAILED;
    }
    if (got_picture == 1) {
        avPacket->pts = frame->getPts();
        avPacket->stream_index = pVideoStream->index;
        avPacket->dts = avPacket->pts;
        avPacket->duration = 1;
        if (av_write_frame(pFormatCtx, avPacket)) {
            return Hw::SUCCESS;
        }
        av_packet_unref(avPacket);
    }
    return Hw::FAILED;
}

bool HwFFmpegEncoder::stop() {
    return false;
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
    if (pCodecCtx) {
        avcodec_close(pCodecCtx);
        pCodecCtx = nullptr;
    }
    pVideoStream = nullptr;
    if (pFormatCtx) {
        avio_close(pFormatCtx->pb);
        avformat_free_context(pFormatCtx);
        pFormatCtx = nullptr;
    }
}