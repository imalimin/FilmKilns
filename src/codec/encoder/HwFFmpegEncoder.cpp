/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwFFmpegEncoder.h"
#include "Logcat.h"
#include "../include/HwVideoFrame.h"
#include "../include/HwAudioFrame.h"

HwFFmpegEncoder::HwFFmpegEncoder() : HwAbsEncoder() {

}

HwFFmpegEncoder::~HwFFmpegEncoder() {
    release();
}

bool HwFFmpegEncoder::prepare(string path, int width, int height) {
    this->path = path;
    this->width = width;
    this->height = height;
    translator = new HwAudioTranslator(HwSampleFormat(HwFrameFormat::HW_SAMPLE_FLTP, 2, 44100),
                                       HwSampleFormat(HwFrameFormat::HW_SAMPLE_S32, 2, 44100));
    return initialize();
}

bool HwFFmpegEncoder::initialize() {
    av_register_all();
    avformat_alloc_output_context2(&pFormatCtx, NULL, "mp4", path.c_str());
    av_dict_set(&pFormatCtx->metadata, "comment", "hwvc", 0);
    if (AV_CODEC_ID_NONE != pFormatCtx->oformat->video_codec) {
        openVideoTrack();
    }
    if (AV_CODEC_ID_NONE != pFormatCtx->oformat->audio_codec) {
        openAudioTrack();
    }
    if (avio_open2(&pFormatCtx->pb, path.c_str(), AVIO_FLAG_WRITE, nullptr, nullptr) < 0) {
        release();
        Logcat::e("HWVC", "HwFFmpegEncoder::initialize failed to open output file!");
        return false;
    }
    avformat_write_header(pFormatCtx, nullptr);
    avFrame = av_frame_alloc();
    avAudioFrame = av_frame_alloc();
    avPacket = av_packet_alloc();
    return true;
}

bool HwFFmpegEncoder::openVideoTrack() {
    AVCodec *pCodec = avcodec_find_encoder(pFormatCtx->oformat->video_codec);
    if (!pCodec) {
        release();
        Logcat::e("HWVC", "HwFFmpegEncoder::initialize could not find %d codec!",
                  pFormatCtx->oformat->video_codec);
        return false;
    }
    pVideoStream = avformat_new_stream(pFormatCtx, nullptr);
    if (nullptr == pVideoStream) {
        release();
        Logcat::e("HWVC", "HwFFmpegEncoder::initialize failed to create video stream!");
        return false;
    }
    pVideoStream->id = pFormatCtx->nb_streams - 1;
    pVideoStream->time_base = {1, 30};
    AVCodecContext *pCodecCtx = pVideoStream->codec;
    configure(pCodecCtx);
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
    /**
     * Important. If not set this, the output will be fail.
     */
    if (pFormatCtx->oformat->flags & AVFMT_GLOBALHEADER) {
        pCodecCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }
    return true;
}

bool HwFFmpegEncoder::openAudioTrack() {
    AVCodec *pCodec = avcodec_find_encoder(pFormatCtx->oformat->audio_codec);
    if (!pCodec) {
        pAudioStream = nullptr;
        Logcat::e("HWVC", "HwFFmpegEncoder::initialize could not find %d codec!",
                  pFormatCtx->oformat->audio_codec);
        return false;
    }
    pAudioStream = avformat_new_stream(pFormatCtx, 0);
    if (nullptr == pAudioStream) {
        release();
        Logcat::e("HWVC", "HwFFmpegEncoder::initialize failed to create audio stream!");
        return false;
    }
    pVideoStream->id = pFormatCtx->nb_streams - 1;
    pAudioStream->time_base = {1, 44100};
    AVCodecContext *pCodecCtx = pAudioStream->codec;
    pCodecCtx->codec_id = pFormatCtx->oformat->audio_codec;
    pCodecCtx->codec_type = AVMEDIA_TYPE_AUDIO;
    pCodecCtx->sample_fmt = AV_SAMPLE_FMT_FLTP;
    pCodecCtx->bit_rate = 64000;
    pCodecCtx->sample_rate = 44100;
    pCodecCtx->channels = 2;
    pCodecCtx->channel_layout = static_cast<uint64_t>(av_get_default_channel_layout(2));
    AVDictionary *opt = nullptr;
    int ret = avcodec_open2(pCodecCtx, pCodec, &opt);
    if (ret < 0) {
        if (pAudioStream && pAudioStream->codec) {
            avcodec_close(pAudioStream->codec);
            pAudioStream->codec = nullptr;
            pAudioStream = nullptr;
        }
        Logcat::e("HWVC", "HwFFmpegEncoder::initialize could not open audio codec: %s",
                  strerror(AVUNERROR(ret)));
        return false;
    }
    if (pFormatCtx->oformat->flags & AVFMT_GLOBALHEADER) {
        pCodecCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }
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

    ctx->time_base = {1, 30};

    ctx->thread_count = 0;
    ctx->qmin = 10;
    ctx->qmax = 30;
    ctx->max_b_frames = 3;
}

HwResult HwFFmpegEncoder::write(HwAbsMediaFrame *frame) {
    AVStream *stream = nullptr;
    int64_t duration = 1;
    if (frame->isAudio() && pAudioStream) {
        stream = pAudioStream;
        duration = static_cast<int64_t>(1.0 / stream->codec->sample_rate * frame->getBufferSize());
        HwAudioFrame *audioFrame = dynamic_cast<HwAudioFrame *>(frame);
        avAudioFrame->data[0] = audioFrame->getBuffer()->getData();
        avAudioFrame->linesize[0] = audioFrame->getBufferSize();
        avAudioFrame->format = HwAbsMediaFrame::convertAudioFrameFormat(frame->getFormat());
        avAudioFrame->sample_rate = audioFrame->getSampleRate();
        avAudioFrame->nb_samples = 1024;
        avAudioFrame->channels = audioFrame->getChannels();
        avAudioFrame->channel_layout = static_cast<uint64_t>(av_get_default_channel_layout(
                audioFrame->getChannels()));
        avFrame->pts = av_rescale_q_rnd(frame->getPts(),
                                        {1, AV_TIME_BASE},
                                        pAudioStream->time_base,
                                        AV_ROUND_NEAR_INF);

        AVFrame *avFrame = nullptr;
        translator->translate(&avFrame, &avAudioFrame);
        avcodec_send_frame(stream->codec, avFrame);
    } else {
        stream = pVideoStream;
        duration = 1;
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
        avFrame->pts = av_rescale_q_rnd(frame->getPts(),
                                        {1, AV_TIME_BASE},
                                        pVideoStream->time_base,
                                        AV_ROUND_NEAR_INF);
        avFrame->pts = frame->getPts();
        avFrame->format = HwAbsMediaFrame::convertVideoFrameFormat(frame->getFormat());

        avcodec_send_frame(stream->codec, avFrame);
    }
    int ret = avcodec_receive_packet(stream->codec, avPacket);
    if (ret < 0) {
        Logcat::e("HWVC", "HwFFmpegEncoder::encode failed %d: %s",
                  stream->index, strerror(AVUNERROR(ret)));
        return Hw::FAILED;
    }
    avPacket->stream_index = stream->index;
    avPacket->duration = duration;
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
    if (avAudioFrame) {
        av_frame_free(&avAudioFrame);
        avAudioFrame = nullptr;
    }
    if (avFrame) {
        av_frame_free(&avFrame);
        avFrame = nullptr;
    }
    if (pAudioStream && pAudioStream->codec) {
        avcodec_close(pAudioStream->codec);
        pAudioStream->codec = nullptr;
        pAudioStream = nullptr;
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
    if (translator) {
        delete translator;
        translator = nullptr;
    }
}